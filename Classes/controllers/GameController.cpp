/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "GameController.h"
#include "../configs/loaders/LevelConfigLoader.h"
#include "../configs/models/LevelConfig.h"
#include "../services/GameModelFromLevelGenerator.h"
#include "../models/GameModel.h"
#include "../views/GameView.h"
#include "../views/PlayFieldView.h"
#include "../views/StackView.h"
#include "../views/BottomCardView.h"
#include "../views/GameResultView.h"
#include "PlayFieldController.h"
#include "StackController.h"
#include "../managers/UndoManager.h"
#include "../models/UndoModel.h"
#include "../models/CardModel.h"
#include "base/CCDirector.h"

USING_NS_CC;

GameController::GameController()
: _gameModel(nullptr)
, _gameView(nullptr)
, _playFieldController(nullptr)
, _stackController(nullptr)
, _undoManager(nullptr)
, _levelId(0)
{
}

GameController::~GameController()
{
    if (_playFieldController != nullptr)
    {
        delete _playFieldController;
        _playFieldController = nullptr;
    }
    if (_stackController != nullptr)
    {
        delete _stackController;
        _stackController = nullptr;
    }
    if (_undoManager != nullptr)
    {
        delete _undoManager;
        _undoManager = nullptr;
    }
    if (_gameModel != nullptr)
    {
        delete _gameModel;
        _gameModel = nullptr;
    }
}

Scene* GameController::startGame(int levelId)
{
    _levelId = levelId;
    
    // 1. 加载关卡配置
    LevelConfig* levelConfig = LevelConfigLoader::loadLevelConfig(levelId);
    if (levelConfig == nullptr)
    {
        return nullptr;
    }
    
    // 2. 生成游戏模型
    _gameModel = GameModelFromLevelGenerator::generateGameModel(levelConfig);
    if (_gameModel == nullptr)
    {
        delete levelConfig;
        return nullptr;
    }
    delete levelConfig;
    
    // 3. 初始化管理器
    _undoManager = new UndoManager();
    _undoManager->init(_gameModel);
    
    // 4. 初始化子控制器
    initControllers();
    
    // 5. 创建游戏视图
    _gameView = GameView::create(_gameModel);
    if (_gameView == nullptr)
    {
        return nullptr;
    }
    
    // 6. 初始化各子控制器和视图
    if (_playFieldController != nullptr && _gameView->getBottomCardView() != nullptr)
    {
        _playFieldController->init(_gameModel, _undoManager, _gameView->getBottomCardView());
        _playFieldController->initView(_gameView->getPlayFieldView());
        // 设置游戏状态检查回调
        _playFieldController->setGameStateCheckCallback([this]() {
            this->checkGameState();
        });
    }
    
    if (_stackController != nullptr && _gameView->getBottomCardView() != nullptr)
    {
        _stackController->init(_gameModel, _undoManager, _gameView->getBottomCardView());
        _stackController->initView(_gameView->getStackView());
        // 设置游戏状态检查回调
        _stackController->setGameStateCheckCallback([this]() {
            this->checkGameState();
        });
    }
    
    // 7. 设置回退按钮回调
    _gameView->setUndoButtonCallback([this]() {
        this->handleUndo();
    });
    
    // 8. 设置重新开始和退出回调
    _gameView->setRestartCallback([this]() {
        this->restartGame();
    });
    
    _gameView->setExitCallback([this]() {
        this->exitGame();
    });
    
    return _gameView;
}

void GameController::initControllers()
{
    // 注意：视图需要在GameView创建后才能初始化
    // 这里先创建控制器对象，但视图引用稍后设置
    _playFieldController = new PlayFieldController();
    _stackController = new StackController();
}

void GameController::handleUndo()
{
    if (_undoManager == nullptr || !_undoManager->canUndo())
    {
        return;  // 没有可撤销的记录
    }
    
    UndoModel* undoModel = _undoManager->popUndoRecord();
    if (undoModel == nullptr)
    {
        return;
    }
    
    // 根据操作类型执行相应的撤销
    bool success = false;
    if (undoModel->getActionType() == UndoActionType::ELIMINATE_CARD)
    {
        if (_playFieldController != nullptr)
        {
            success = _playFieldController->performUndo(undoModel);
        }
    }
    else if (undoModel->getActionType() == UndoActionType::REPLACE_BOTTOM_CARD)
    {
        if (_stackController != nullptr)
        {
            success = _stackController->performUndo(undoModel);
        }
    }
    
    if (!success)
    {
        // 撤销失败，重新放回栈中
        _undoManager->pushUndoRecord(undoModel);
    }
    else
    {
        delete undoModel;
        // 撤销后检查游戏状态
        checkGameState();
    }
}

void GameController::checkGameState()
{
    if (_gameModel == nullptr || _gameView == nullptr)
    {
        return;
    }
    
    // 检查胜利条件：主牌区的牌消耗完了
    const std::vector<int>& playFieldCardIds = _gameModel->getPlayFieldCardIds();
    if (playFieldCardIds.empty())
    {
        // 显示闯关成功弹窗
        _gameView->showGameResult(GameResultType::VICTORY);
        return;
    }
    
    // 检查失败条件：备选牌区消耗完了，且主牌区没有与底牌匹配的牌
    const std::vector<int>& stackCardIds = _gameModel->getStackCardIds();
    if (stackCardIds.empty())
    {
        int bottomCardId = _gameModel->getBottomCardId();
        if (bottomCardId == 0)
        {
            return;  // 没有底牌，无法判断
        }
        
        const CardModel* bottomCard = _gameModel->getCardById(bottomCardId);
        if (bottomCard == nullptr)
        {
            return;
        }
        
        // 检查主牌区是否有与底牌匹配的牌
        bool hasMatch = false;
        for (int cardId : playFieldCardIds)
        {
            const CardModel* card = _gameModel->getCardById(cardId);
            if (card != nullptr && card->canMatchWith(bottomCard))
            {
                hasMatch = true;
                break;
            }
        }
        
        if (!hasMatch)
        {
            // 显示闯关失败弹窗
            _gameView->showGameResult(GameResultType::DEFEAT);
        }
    }
}

void GameController::restartGame()
{
    // 重新开始游戏：使用相同的关卡ID，不删除GameController，只重新生成数据
    
    // 1. 先清理回调并移除弹窗，避免悬空指针
    if (_gameView != nullptr)
    {
        // 移除游戏结果弹窗（如果存在）
        _gameView->removeGameResultView();
        // 清理回调
        _gameView->setRestartCallback(nullptr);
        _gameView->setExitCallback(nullptr);
        _gameView->setUndoButtonCallback(nullptr);
    }
    
    // 2. 清理旧数据
    if (_playFieldController != nullptr)
    {
        delete _playFieldController;
        _playFieldController = nullptr;
    }
    if (_stackController != nullptr)
    {
        delete _stackController;
        _stackController = nullptr;
    }
    if (_undoManager != nullptr)
    {
        delete _undoManager;
        _undoManager = nullptr;
    }
    if (_gameModel != nullptr)
    {
        delete _gameModel;
        _gameModel = nullptr;
    }
    
    // 3. 移除旧的游戏视图（但不删除GameController）
    if (_gameView != nullptr)
    {
        // 先取消所有定时器和动作，避免回调执行时对象已删除
        _gameView->unscheduleAllSelectors();
        _gameView->stopAllActions();
        _gameView->removeFromParent();
        _gameView = nullptr;
    }
    
    // 4. 重新生成游戏数据
    LevelConfig* levelConfig = LevelConfigLoader::loadLevelConfig(_levelId);
    if (levelConfig == nullptr)
    {
        return;
    }
    
    _gameModel = GameModelFromLevelGenerator::generateGameModel(levelConfig);
    delete levelConfig;
    
    if (_gameModel == nullptr)
    {
        return;
    }
    
    // 5. 重新初始化管理器
    _undoManager = new UndoManager();
    _undoManager->init(_gameModel);
    
    // 6. 重新初始化子控制器
    initControllers();
    
    // 7. 创建新的游戏视图
    _gameView = GameView::create(_gameModel);
    if (_gameView == nullptr)
    {
        return;
    }
    
    // 8. 初始化各子控制器和视图
    if (_playFieldController != nullptr && _gameView->getBottomCardView() != nullptr)
    {
        _playFieldController->init(_gameModel, _undoManager, _gameView->getBottomCardView());
        _playFieldController->initView(_gameView->getPlayFieldView());
        // 设置游戏状态检查回调
        _playFieldController->setGameStateCheckCallback([this]() {
            this->checkGameState();
        });
    }
    
    if (_stackController != nullptr && _gameView->getBottomCardView() != nullptr)
    {
        _stackController->init(_gameModel, _undoManager, _gameView->getBottomCardView());
        _stackController->initView(_gameView->getStackView());
        // 设置游戏状态检查回调
        _stackController->setGameStateCheckCallback([this]() {
            this->checkGameState();
        });
    }
    
    // 9. 设置回退按钮回调
    _gameView->setUndoButtonCallback([this]() {
        this->handleUndo();
    });
    
    // 10. 设置重新开始和退出回调
    _gameView->setRestartCallback([this]() {
        this->restartGame();
    });
    
    _gameView->setExitCallback([this]() {
        this->exitGame();
    });
    
    // 11. 替换场景
    Director::getInstance()->replaceScene(_gameView);
}

void GameController::exitGame()
{
    // 退出游戏
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

