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

#include "BottomCardView.h"
#include "../models/GameModel.h"
#include "../models/CardModel.h"

USING_NS_CC;

BottomCardView* BottomCardView::create(const GameModel* gameModel)
{
    BottomCardView* view = new BottomCardView();
    if (view && view->init(gameModel))
    {
        view->autorelease();
        return view;
    }
    delete view;
    return nullptr;
}

bool BottomCardView::init(const GameModel* gameModel)
{
    if (!Node::init() || gameModel == nullptr)
    {
        return false;
    }
    
    _gameModel = gameModel;
    _cardView = nullptr;
    _cardSize = Size(100, 140);
    
    // 底牌视图区域：540*580，底牌居中显示
    this->setContentSize(Size(540, 580));
    
    updateView(gameModel);
    
    return true;
}

void BottomCardView::updateView(const GameModel* gameModel)
{
    if (gameModel == nullptr)
    {
        return;
    }
    
    _gameModel = gameModel;
    
    // 移除现有卡牌视图
    if (_cardView != nullptr)
    {
        _cardView->removeFromParent();
        _cardView = nullptr;
    }
    
    // 创建新的卡牌视图
    int bottomCardId = gameModel->getBottomCardId();
    if (bottomCardId > 0)
    {
        const CardModel* cardModel = gameModel->getCardById(bottomCardId);
        if (cardModel != nullptr)
        {
            _cardView = CardView::create(cardModel);
            if (_cardView != nullptr)
            {
                // 底牌在区域中居中显示，与备选牌堆保持水平对齐
                Size viewSize = this->getContentSize();
                // 使用与备选牌堆相同的Y坐标计算方式（视图高度的一半）
                _cardView->setPosition(Vec2(viewSize.width * 0.5f, viewSize.height * 0.5f));
                this->addChild(_cardView);
            }
        }
    }
}

void BottomCardView::playReplaceAnimation(int cardId, float duration, const std::function<void()>& callback)
{
    // 创建新卡牌视图
    if (_gameModel != nullptr)
    {
        const CardModel* cardModel = _gameModel->getCardById(cardId);
        if (cardModel != nullptr)
        {
            CardView* newCardView = CardView::create(cardModel);
            if (newCardView != nullptr)
            {
                newCardView->setPosition(Vec2(_cardSize.width / 2, _cardSize.height / 2));
                newCardView->setOpacity(0);
                this->addChild(newCardView);
                
                // 淡入动画
                auto fadeIn = FadeIn::create(duration);
                if (callback != nullptr)
                {
                    auto callFunc = CallFunc::create([this, newCardView, callback]() {
                        if (_cardView != nullptr)
                        {
                            _cardView->removeFromParent();
                        }
                        _cardView = newCardView;
                        callback();
                    });
                    newCardView->runAction(Sequence::create(fadeIn, callFunc, nullptr));
                }
                else
                {
                    auto callFunc = CallFunc::create([this, newCardView]() {
                        if (_cardView != nullptr)
                        {
                            _cardView->removeFromParent();
                        }
                        _cardView = newCardView;
                    });
                    newCardView->runAction(Sequence::create(fadeIn, callFunc, nullptr));
                }
            }
        }
    }
}

void BottomCardView::playUndoAnimation(int cardId, float duration, const std::function<void()>& callback)
{
    // 直接更新视图即可
    updateView(_gameModel);
    if (callback != nullptr)
    {
        auto callFunc = CallFunc::create(callback);
        this->runAction(Sequence::create(DelayTime::create(duration), callFunc, nullptr));
    }
}

