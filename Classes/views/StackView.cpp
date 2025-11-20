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

#include "StackView.h"
#include "../models/GameModel.h"
#include "../models/CardModel.h"
#include "base/CCEventListenerTouch.h"
#include "base/CCEventDispatcher.h"

USING_NS_CC;

StackView* StackView::create(const GameModel* gameModel)
{
    StackView* view = new StackView();
    if (view && view->init(gameModel))
    {
        view->autorelease();
        return view;
    }
    delete view;
    return nullptr;
}

bool StackView::init(const GameModel* gameModel)
{
    if (!Layer::init() || gameModel == nullptr)
    {
        return false;
    }
    
    _gameModel = gameModel;
    // 从第一张卡牌获取实际尺寸（如果存在）
    _cardSize = Size(100, 140);  // 默认值，会在updateView中更新
    _cardSpacing = 50.0f;  // 卡牌重叠间距
    
    // 设置触摸监听
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(StackView::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(StackView::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // 不在init中调用updateView，因为此时视图尺寸还未设置
    // updateView会在GameView设置完视图尺寸后调用
    
    return true;
}

void StackView::updateView(const GameModel* gameModel)
{
    if (gameModel == nullptr)
    {
        return;
    }
    
    _gameModel = gameModel;
    
    const std::vector<int>& stackCardIds = gameModel->getStackCardIds();
    
    // 先创建第一张卡牌，获取实际卡牌尺寸（如果还没有获取）
    if (!stackCardIds.empty() && _cardSize.width <= 0)
    {
        int firstCardId = stackCardIds[0];
        const CardModel* firstCardModel = gameModel->getCardById(firstCardId);
        if (firstCardModel != nullptr)
        {
            CardView* tempCardView = CardView::create(firstCardModel);
            if (tempCardView != nullptr)
            {
                _cardSize = tempCardView->getCardSize();
                tempCardView->removeFromParent();
                tempCardView->release();  // 释放临时创建的卡牌
            }
        }
    }
    
    // 如果是首次创建，记录所有卡牌的原始索引
    if (_cardOriginalIndex.empty() && !stackCardIds.empty())
    {
        for (size_t i = 0; i < stackCardIds.size(); ++i)
        {
            _cardOriginalIndex[stackCardIds[i]] = (int)i;
        }
    }
    
    // 移除已不存在的卡牌视图
    std::vector<int> cardsToRemove;
    for (auto& pair : _cardViews)
    {
        int cardId = pair.first;
        if (std::find(stackCardIds.begin(), stackCardIds.end(), cardId) == stackCardIds.end())
        {
            cardsToRemove.push_back(cardId);
        }
    }
    for (int cardId : cardsToRemove)
    {
        auto it = _cardViews.find(cardId);
        if (it != _cardViews.end())
        {
            it->second->removeFromParent();
            _cardViews.erase(it);
        }
        _cardOriginalIndex.erase(cardId);
    }
    
    // 创建或更新现有卡牌视图，使用原始索引计算位置
    for (size_t i = 0; i < stackCardIds.size(); ++i)
    {
        int cardId = stackCardIds[i];
        const CardModel* cardModel = gameModel->getCardById(cardId);
        if (cardModel == nullptr)
        {
            continue;
        }
        
        // 如果卡牌视图不存在，创建它
        auto it = _cardViews.find(cardId);
        if (it == _cardViews.end())
        {
            CardView* cardView = CardView::create(cardModel);
            if (cardView != nullptr)
            {
                // 如果这是第一张卡牌，更新卡牌尺寸
                if (i == 0 && _cardSize.width <= 0)
                {
                    _cardSize = cardView->getCardSize();
                }
                
                // 获取原始索引（如果不存在，使用当前索引）
                int originalIndex = _cardOriginalIndex.find(cardId) != _cardOriginalIndex.end() 
                    ? _cardOriginalIndex[cardId] 
                    : (int)i;
                
                Vec2 pos = getCardPosition(originalIndex);
                cardView->setPosition(pos);
                // 设置Z轴顺序：后面的牌在上层（使用原始索引）
                this->addChild(cardView, originalIndex);
                _cardViews[cardId] = cardView;
            }
        }
        else
        {
            // 卡牌视图已存在，更新位置（使用原始索引）
            CardView* cardView = it->second;
            int originalIndex = _cardOriginalIndex.find(cardId) != _cardOriginalIndex.end() 
                ? _cardOriginalIndex[cardId] 
                : (int)i;
            Vec2 pos = getCardPosition(originalIndex);
            cardView->setPosition(pos);
            // 更新Z轴顺序
            cardView->setLocalZOrder(originalIndex);
        }
    }
}

void StackView::playCardMoveAnimation(int cardId, const Vec2& targetPos, 
                                    float duration, const std::function<void()>& callback)
{
    CardView* cardView = getCardView(cardId);
    if (cardView != nullptr)
    {
        cardView->playMoveAnimation(targetPos, duration, callback);
    }
}

void StackView::playUndoAnimation(int cardId, const Vec2& targetPos, 
                                  float duration, const std::function<void()>& callback)
{
    CardView* cardView = getCardView(cardId);
    if (cardView != nullptr)
    {
        cardView->playMoveAnimation(targetPos, duration, callback);
    }
}

CardView* StackView::getCardView(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end())
    {
        return it->second;
    }
    return nullptr;
}


Vec2 StackView::getCardPosition(int index)
{
    // 备用牌堆：卡牌重叠显示，从左侧开始，与底牌保持水平对齐
    Size viewSize = this->getContentSize();
    float startX = 50.0f;  // 左侧起始位置
    // 确保与底牌水平对齐：使用视图高度的一半作为Y坐标
    float startY = viewSize.height * 0.5f;
    
    // 卡牌重叠效果：每张牌向右偏移一定距离（约卡牌宽度的1/4）
    float overlapOffset = _cardSize.width * 0.25f;
    float x = startX + index * overlapOffset;
    float y = startY;  // 所有卡牌使用相同的Y坐标，保持水平对齐
    
    return Vec2(x, y);
}

bool StackView::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void StackView::onTouchEnded(Touch* touch, Event* event)
{
    if (_cardClickCallback == nullptr || _gameModel == nullptr)
    {
        return;
    }
    
    Vec2 touchPos = touch->getLocation();
    Vec2 localPos = this->convertToNodeSpace(touchPos);
    
    // 检查点击的是哪张卡牌（从顶部到底部检查，只点击顶部牌）
    const std::vector<int>& stackCardIds = _gameModel->getStackCardIds();
    if (stackCardIds.empty())
    {
        return;
    }
    
    // 只检查顶部牌（最后一张）
    int topCardId = stackCardIds.back();
    CardView* cardView = getCardView(topCardId);
    if (cardView != nullptr)
    {
        Vec2 cardPos = cardView->getPosition();
        Size cardSize = cardView->getCardSize();
        Rect cardRect = Rect(cardPos.x - cardSize.width / 2,
                            cardPos.y - cardSize.height / 2,
                            cardSize.width, cardSize.height);
        
        if (cardRect.containsPoint(localPos))
        {
            _cardClickCallback(topCardId);
        }
    }
}

