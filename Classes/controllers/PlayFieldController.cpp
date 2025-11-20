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

#include "PlayFieldController.h"
#include "../models/GameModel.h"
#include "../models/CardModel.h"
#include "../models/UndoModel.h"
#include "../views/PlayFieldView.h"
#include "../views/BottomCardView.h"
#include "../views/CardView.h"
#include "../managers/UndoManager.h"
#include "../utils/CommonUtils.h"
#include <algorithm>

USING_NS_CC;

PlayFieldController::PlayFieldController()
: _gameModel(nullptr)
, _playFieldView(nullptr)
, _bottomCardView(nullptr)
, _undoManager(nullptr)
{
}

PlayFieldController::~PlayFieldController()
{
}

void PlayFieldController::init(GameModel* gameModel, UndoManager* undoManager, BottomCardView* bottomCardView)
{
    _gameModel = gameModel;
    _undoManager = undoManager;
    _bottomCardView = bottomCardView;
}

void PlayFieldController::initView(PlayFieldView* playFieldView)
{
    _playFieldView = playFieldView;
    
    if (_playFieldView != nullptr)
    {
        // 设置卡牌点击回调
        _playFieldView->setCardClickCallback([this](int cardId) {
            this->handleCardClick(cardId);
        });
    }
}

bool PlayFieldController::handleCardClick(int cardId)
{
    if (_gameModel == nullptr || _playFieldView == nullptr || _bottomCardView == nullptr)
    {
        return false;
    }
    
    // 获取底牌
    int bottomCardId = _gameModel->getBottomCardId();
    if (bottomCardId == 0)
    {
        return false;  // 没有底牌
    }
    
    // 获取点击的卡牌和底牌
    CardModel* clickedCard = _gameModel->getCardById(cardId);
    const CardModel* bottomCard = _gameModel->getCardById(bottomCardId);
    
    if (clickedCard == nullptr || bottomCard == nullptr)
    {
        return false;
    }
    
    // 检查是否匹配（点数差1）
    if (!clickedCard->canMatchWith(bottomCard))
    {
        return false;  // 不匹配，不能消除
    }
    
    // 记录撤销信息
    UndoModel* undoModel = new UndoModel();
    undoModel->setActionType(UndoActionType::ELIMINATE_CARD);
    undoModel->setCardId(cardId);  // 新底牌（主牌堆的牌）
    undoModel->setReplacedCardId(bottomCardId);  // 被替换的底牌
    
    // 保存卡牌数据
    undoModel->setCardModel(CommonUtils::cloneCardModel(clickedCard));
    undoModel->setReplacedCardModel(CommonUtils::cloneCardModel(bottomCard));
    
    // 保存位置信息
    CardView* clickedCardView = _playFieldView->getCardView(cardId);
    CardView* bottomCardView = _bottomCardView->getCardView();
    if (clickedCardView != nullptr)
    {
        undoModel->setCardPosition(clickedCardView->getPosition());
    }
    if (bottomCardView != nullptr)
    {
        undoModel->setReplacedCardPosition(bottomCardView->getPosition());
    }
    
    // 更新模型：从主牌堆移除该卡牌，设置为新的底牌
    _gameModel->removeFromPlayField(cardId);
    _gameModel->setBottomCardId(cardId);
    
    // 播放替换动画：主牌堆的牌平移到底牌位置
    if (clickedCardView != nullptr && bottomCardView != nullptr)
    {
        Vec2 targetPos = bottomCardView->getPosition();
        // 转换为世界坐标
        Vec2 worldPos = _bottomCardView->convertToWorldSpace(targetPos);
        Vec2 localPos = _playFieldView->convertToNodeSpace(worldPos);
        
        clickedCardView->playMoveAnimation(localPos, 0.3f, [this, cardId]() {
            // 动画完成后更新视图
            if (_playFieldView != nullptr && _bottomCardView != nullptr && _gameModel != nullptr)
            {
                _playFieldView->updateView(_gameModel);
                _bottomCardView->updateView(_gameModel);
                // 检查游戏状态
                if (_gameStateCheckCallback != nullptr)
                {
                    _gameStateCheckCallback();
                }
            }
        });
    }
    else
    {
        // 直接更新视图
        _playFieldView->updateView(_gameModel);
        _bottomCardView->updateView(_gameModel);
        // 检查游戏状态
        if (_gameStateCheckCallback != nullptr)
        {
            _gameStateCheckCallback();
        }
    }
    
    // 添加到撤销记录
    if (_undoManager != nullptr)
    {
        _undoManager->pushUndoRecord(undoModel);
    }
    
    return true;
}

bool PlayFieldController::performUndo(UndoModel* undoModel)
{
    if (undoModel == nullptr || _gameModel == nullptr || _playFieldView == nullptr || _bottomCardView == nullptr)
    {
        return false;
    }
    
    if (undoModel->getActionType() != UndoActionType::ELIMINATE_CARD)
    {
        return false;
    }
    
    int newCardId = undoModel->getCardId();  // 主牌堆的牌（现在是底牌）
    int oldCardId = undoModel->getReplacedCardId();  // 原底牌
    
    // 恢复模型：将底牌恢复，主牌堆恢复卡牌
    _gameModel->setBottomCardId(oldCardId);
    std::vector<int>& playFieldCardIds = _gameModel->getPlayFieldCardIds();
    playFieldCardIds.push_back(newCardId);
    
    // 更新视图
    _playFieldView->updateView(_gameModel);
    _bottomCardView->updateView(_gameModel);
    
    return true;
}
