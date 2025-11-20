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

#include "StackController.h"
#include "../models/GameModel.h"
#include "../models/CardModel.h"
#include "../models/UndoModel.h"
#include "../views/StackView.h"
#include "../views/BottomCardView.h"
#include "../views/CardView.h"
#include "../managers/UndoManager.h"
#include "../utils/CommonUtils.h"
#include <algorithm>

USING_NS_CC;

StackController::StackController()
: _gameModel(nullptr)
, _stackView(nullptr)
, _bottomCardView(nullptr)
, _undoManager(nullptr)
{
}

StackController::~StackController()
{
}

void StackController::init(GameModel* gameModel, UndoManager* undoManager, BottomCardView* bottomCardView)
{
    _gameModel = gameModel;
    _undoManager = undoManager;
    _bottomCardView = bottomCardView;
}

void StackController::initView(StackView* stackView)
{
    _stackView = stackView;
    
    if (_stackView != nullptr)
    {
        // 设置卡牌点击回调
        _stackView->setCardClickCallback([this](int cardId) {
            this->handleCardClick(cardId);
        });
    }
}

bool StackController::handleCardClick(int cardId)
{
    if (_gameModel == nullptr || _stackView == nullptr || _bottomCardView == nullptr)
    {
        return false;
    }
    
    std::vector<int>& stackCardIds = _gameModel->getStackCardIds();
    if (stackCardIds.empty())
    {
        return false;
    }
    
    // 只能点击顶部牌
    int topCardId = stackCardIds.back();
    if (cardId != topCardId)
    {
        return false;  // 只能点击顶部牌
    }
    
    // 获取当前底牌
    int currentBottomCardId = _gameModel->getBottomCardId();
    
    // 记录撤销信息
    UndoModel* undoModel = new UndoModel();
    undoModel->setActionType(UndoActionType::REPLACE_BOTTOM_CARD);
    undoModel->setCardId(topCardId);  // 新底牌
    undoModel->setReplacedCardId(currentBottomCardId);  // 被替换的底牌
    
    // 保存卡牌数据
    CardModel* topCard = _gameModel->getCardById(topCardId);
    const CardModel* bottomCard = _gameModel->getCardById(currentBottomCardId);
    if (topCard != nullptr)
    {
        undoModel->setCardModel(CommonUtils::cloneCardModel(topCard));
    }
    if (bottomCard != nullptr)
    {
        undoModel->setReplacedCardModel(CommonUtils::cloneCardModel(bottomCard));
    }
    
    // 保存位置信息
    CardView* topCardView = _stackView->getCardView(topCardId);
    if (topCardView != nullptr)
    {
        undoModel->setCardPosition(topCardView->getPosition());
    }
    CardView* bottomCardView = _bottomCardView->getCardView();
    if (bottomCardView != nullptr)
    {
        undoModel->setReplacedCardPosition(bottomCardView->getPosition());
    }
    
    // 更新模型：从备用牌堆移除顶部牌，设置为新的底牌
    _gameModel->removeTopFromStack();
    _gameModel->setBottomCardId(topCardId);
    
    // 播放替换动画：备用牌堆的牌平移到底牌位置
    if (topCardView != nullptr && bottomCardView != nullptr)
    {
        Vec2 targetPos = bottomCardView->getPosition();
        // 转换为世界坐标
        Vec2 worldPos = _bottomCardView->convertToWorldSpace(targetPos);
        Vec2 localPos = _stackView->convertToNodeSpace(worldPos);
        
        topCardView->playMoveAnimation(localPos, 0.3f, [this, topCardId]() {
            // 动画完成后更新视图
            if (_stackView != nullptr && _bottomCardView != nullptr && _gameModel != nullptr)
            {
                _stackView->updateView(_gameModel);
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
        _stackView->updateView(_gameModel);
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

bool StackController::performUndo(UndoModel* undoModel)
{
    if (undoModel == nullptr || _gameModel == nullptr || _stackView == nullptr || _bottomCardView == nullptr)
    {
        return false;
    }
    
    if (undoModel->getActionType() != UndoActionType::REPLACE_BOTTOM_CARD)
    {
        return false;
    }
    
    int newCardId = undoModel->getCardId();
    int oldCardId = undoModel->getReplacedCardId();
    
    // 恢复模型：将底牌恢复，备用牌堆恢复顶部牌
    _gameModel->setBottomCardId(oldCardId);
    _gameModel->addToStackTop(newCardId);
    
    // 更新视图
    _stackView->updateView(_gameModel);
    _bottomCardView->updateView(_gameModel);
    
    return true;
}
