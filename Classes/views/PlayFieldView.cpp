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

#include "PlayFieldView.h"
#include "../models/GameModel.h"
#include "../models/CardModel.h"
#include "base/CCEventListenerTouch.h"
#include "base/CCEventDispatcher.h"

USING_NS_CC;

PlayFieldView* PlayFieldView::create(const GameModel* gameModel)
{
    PlayFieldView* view = new PlayFieldView();
    if (view && view->init(gameModel))
    {
        view->autorelease();
        return view;
    }
    delete view;
    return nullptr;
}

bool PlayFieldView::init(const GameModel* gameModel)
{
    if (!Layer::init() || gameModel == nullptr)
    {
        return false;
    }
    
    _gameModel = gameModel;
    _cardSize = Size(100, 140);
    // 卡牌间距：分散排布，间距大一点
    _cardSpacingX = 250.0f;  // 横向间距（增大间距）
    _cardSpacingY = 400.0f;  // 纵向间距（增大间距）
    
    // 设置触摸监听
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(PlayFieldView::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(PlayFieldView::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // 不在init中调用updateView，因为此时视图尺寸还未设置
    // updateView会在GameView设置完视图尺寸后调用
    
    return true;
}

void PlayFieldView::updateView(const GameModel* gameModel)
{
    if (gameModel == nullptr)
    {
        return;
    }
    
    _gameModel = gameModel;
    
    const std::vector<int>& playFieldCardIds = gameModel->getPlayFieldCardIds();
    
    // 先创建第一张卡牌，获取实际卡牌尺寸（如果还没有获取）
    if (!playFieldCardIds.empty() && _cardSize.width <= 0)
    {
        int firstCardId = playFieldCardIds[0];
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
    if (_cardOriginalIndex.empty() && !playFieldCardIds.empty())
    {
        for (size_t i = 0; i < playFieldCardIds.size(); ++i)
        {
            _cardOriginalIndex[playFieldCardIds[i]] = (int)i;
        }
    }
    
    // 移除已不存在的卡牌视图
    std::vector<int> cardsToRemove;
    for (auto& pair : _cardViews)
    {
        int cardId = pair.first;
        if (std::find(playFieldCardIds.begin(), playFieldCardIds.end(), cardId) == playFieldCardIds.end())
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
    for (size_t i = 0; i < playFieldCardIds.size(); ++i)
    {
        int cardId = playFieldCardIds[i];
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
                this->addChild(cardView);
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
        }
    }
}

void PlayFieldView::playCardMoveAnimation(int cardId, const Vec2& targetPos, 
                                         float duration, const std::function<void()>& callback)
{
    CardView* cardView = getCardView(cardId);
    if (cardView != nullptr)
    {
        cardView->playMoveAnimation(targetPos, duration, callback);
    }
}

void PlayFieldView::playUndoAnimation(int cardId, const Vec2& targetPos, 
                                     float duration, const std::function<void()>& callback)
{
    CardView* cardView = getCardView(cardId);
    if (cardView != nullptr)
    {
        cardView->playMoveAnimation(targetPos, duration, callback);
    }
}

CardView* PlayFieldView::getCardView(int cardId)
{
    auto it = _cardViews.find(cardId);
    if (it != _cardViews.end())
    {
        return it->second;
    }
    return nullptr;
}

bool PlayFieldView::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void PlayFieldView::onTouchEnded(Touch* touch, Event* event)
{
    if (_cardClickCallback == nullptr || _gameModel == nullptr)
    {
        return;
    }
    
    Vec2 touchPos = touch->getLocation();
    Vec2 localPos = this->convertToNodeSpace(touchPos);
    
    // 检查点击的是哪张卡牌
    const std::vector<int>& playFieldCardIds = _gameModel->getPlayFieldCardIds();
    for (int cardId : playFieldCardIds)
    {
        CardView* cardView = getCardView(cardId);
        if (cardView != nullptr)
        {
            Vec2 cardPos = cardView->getPosition();
            Size cardSize = cardView->getCardSize();
            Rect cardRect = Rect(cardPos.x - cardSize.width / 2,
                                cardPos.y - cardSize.height / 2,
                                cardSize.width, cardSize.height);
            
            if (cardRect.containsPoint(localPos))
            {
                _cardClickCallback(cardId);
                break;
            }
        }
    }
}

Vec2 PlayFieldView::getCardPosition(int index)
{
    // 主牌堆：8张牌，矩形排布（2行4列），均匀排布，位于上方区域中间
    Size viewSize = this->getContentSize();
    
    // 如果视图尺寸无效，使用默认值
    if (viewSize.width <= 0 || viewSize.height <= 0)
    {
        viewSize = Size(1080, 1500);  // 默认尺寸
    }
    
    // 固定2行4列布局
    int cols = 4;  // 每行4张牌
    int rows = 2;  // 共2行
    
    int row = index / cols;
    int col = index % cols;
    
    // 计算总宽度和总高度（包括卡牌尺寸和间距）
    float totalWidth = (cols - 1) * _cardSpacingX + _cardSize.width;
    float totalHeight = (rows - 1) * _cardSpacingY + _cardSize.height;
    
    // 计算起始位置（在视图中间居中显示，均匀排布）
    float startX = (viewSize.width - totalWidth) / 2.0f + _cardSize.width / 2.0f;
    float startY = (viewSize.height + totalHeight) / 2.0f - _cardSize.height / 2.0f;  // 垂直居中
    
    // 均匀排布：每张牌的位置基于起始位置和间距计算
    float x = startX + col * _cardSpacingX;
    float y = startY - row * _cardSpacingY;
    
    return Vec2(x, y);
}

