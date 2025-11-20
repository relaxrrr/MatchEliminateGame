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

#include "CardView.h"
#include "../models/CardModel.h"
#include "../configs/models/CardResConfig.h"
#include "2d/CCDrawNode.h"
#include "2d/CCSprite.h"
#include "renderer/CCTextureCache.h"
#include "base/CCEventListenerTouch.h"
#include "base/CCEventDispatcher.h"
#include "base/CCDirector.h"

CardView* CardView::create(const CardModel* cardModel)
{
    CardView* view = new CardView();
    if (view && view->init(cardModel))
    {
        view->autorelease();
        return view;
    }
    delete view;
    return nullptr;
}

bool CardView::init(const CardModel* cardModel)
{
    if (cardModel == nullptr)
    {
        return false;
    }
    
    if (!Node::init())
    {
        return false;
    }
    
    _cardModel = cardModel;
    _cardId = cardModel->getCardId();
    
    // 从card_general.png获取实际卡牌尺寸
    std::string bgPath = CardResConfig::getInstance()->getCardBackgroundPath();
    auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(bgPath);
    if (texture != nullptr)
    {
        cocos2d::Size textureSize = texture->getContentSize();
        _cardSize = textureSize;
    }
    else
    {
        // 如果加载失败，使用默认尺寸
        _cardSize = cocos2d::Size(100, 140);
    }
    _backgroundSprite = nullptr;
    _smallNumberSprite = nullptr;
    _suitSprite = nullptr;
    _bigNumberSprite = nullptr;
    
    // 设置内容尺寸
    this->setContentSize(_cardSize);
    
    // 设置锚点为(0.5, 0.5)，方便定位
    this->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    
    // 创建卡牌UI元素
    createCardElements();
    
    return true;
}

void CardView::updateCard(const CardModel* cardModel)
{
    if (cardModel == nullptr || cardModel->getCardId() != _cardId)
    {
        return;
    }
    
    _cardModel = cardModel;
    
    // 更新卡牌UI元素
    updateCardElements();
}

void CardView::playMoveAnimation(const cocos2d::Vec2& targetPos, float duration, 
                                const std::function<void()>& callback)
{
    auto moveAction = cocos2d::MoveTo::create(duration, targetPos);
    
    if (callback != nullptr)
    {
        auto callFunc = cocos2d::CallFunc::create(callback);
        this->runAction(cocos2d::Sequence::create(moveAction, callFunc, nullptr));
    }
    else
    {
        this->runAction(moveAction);
    }
}

void CardView::createCardElements()
{
    CardResConfig* resConfig = CardResConfig::getInstance();
    
    // 创建背景（白色，圆角）
    std::string bgPath = resConfig->getCardBackgroundPath();
    _backgroundSprite = cocos2d::Sprite::create(bgPath);
    if (_backgroundSprite != nullptr)
    {
        _backgroundSprite->setPosition(cocos2d::Vec2(_cardSize.width / 2, _cardSize.height / 2));
        _backgroundSprite->setContentSize(_cardSize);
        this->addChild(_backgroundSprite, 0);
    }
    else
    {
        // 如果背景加载失败，创建白色圆角背景
        auto drawNode = cocos2d::DrawNode::create();
        float cornerRadius = 8.0f;  // 圆角半径
        // 绘制圆角矩形（简化版：绘制白色矩形）
        drawNode->drawSolidRect(cocos2d::Vec2::ZERO, 
                               cocos2d::Vec2(_cardSize.width, _cardSize.height), 
                               cocos2d::Color4F(1.0f, 1.0f, 1.0f, 1.0f));
        drawNode->setPosition(cocos2d::Vec2::ZERO);
        this->addChild(drawNode, 0);
    }
    
    bool isRed = resConfig->isRedSuit(_cardModel->getSuit());
    
    // 创建左上角小数字（位置：左上角，小尺寸）
    std::string smallNumPath = resConfig->getSmallNumberImagePath(_cardModel->getFace(), isRed);
    _smallNumberSprite = cocos2d::Sprite::create(smallNumPath);
    if (_smallNumberSprite != nullptr)
    {
        // 左上角位置，距离边缘约8-10%
        float offsetX = _cardSize.width * 0.08f;
        float offsetY = _cardSize.height * 0.92f;
        _smallNumberSprite->setPosition(cocos2d::Vec2(offsetX, offsetY));
        // 小数字尺寸约为卡牌宽度的12-15%
        float smallSize = _cardSize.width * 0.13f;
        _smallNumberSprite->setScale(smallSize / _smallNumberSprite->getContentSize().width);
        this->addChild(_smallNumberSprite, 2);
    }
    
    // 创建左上角花色（紧邻小数字右侧）
    std::string suitPath = resConfig->getSuitImagePath(_cardModel->getSuit());
    _suitSprite = cocos2d::Sprite::create(suitPath);
    if (_suitSprite != nullptr)
    {
        // 花色在小数字右侧，稍微偏上
        float offsetX = _cardSize.width * 0.22f;  // 小数字右侧
        float offsetY = _cardSize.height * 0.92f;
        _suitSprite->setPosition(cocos2d::Vec2(offsetX, offsetY));
        // 花色尺寸约为卡牌宽度的10-12%
        float suitSize = _cardSize.width * 0.11f;
        _suitSprite->setScale(suitSize / _suitSprite->getContentSize().width);
        this->addChild(_suitSprite, 2);
    }
    
    // 创建中央大数字（居中，大尺寸，有光泽效果）
    std::string bigNumPath = resConfig->getBigNumberImagePath(_cardModel->getFace(), isRed);
    _bigNumberSprite = cocos2d::Sprite::create(bigNumPath);
    if (_bigNumberSprite != nullptr)
    {
        // 居中位置，稍微偏下一点（视觉中心）
        float centerX = _cardSize.width / 2;
        float centerY = _cardSize.height * 0.48f;  // 稍微偏下，视觉中心
        _bigNumberSprite->setPosition(cocos2d::Vec2(centerX, centerY));
        // 大数字尺寸约为卡牌宽度的35-40%
        float bigSize = _cardSize.width * 0.38f;
        _bigNumberSprite->setScale(bigSize / _bigNumberSprite->getContentSize().width);
        this->addChild(_bigNumberSprite, 1);
    }
}

void CardView::updateCardElements()
{
    CardResConfig* resConfig = CardResConfig::getInstance();
    bool isRed = resConfig->isRedSuit(_cardModel->getSuit());
    
    // 更新左上角小数字
    if (_smallNumberSprite != nullptr)
    {
        std::string smallNumPath = resConfig->getSmallNumberImagePath(_cardModel->getFace(), isRed);
        auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(smallNumPath);
        if (texture != nullptr)
        {
            _smallNumberSprite->setTexture(texture);
            // 保持缩放比例
            float smallSize = _cardSize.width * 0.13f;
            _smallNumberSprite->setScale(smallSize / _smallNumberSprite->getContentSize().width);
        }
    }
    
    // 更新左上角花色
    if (_suitSprite != nullptr)
    {
        std::string suitPath = resConfig->getSuitImagePath(_cardModel->getSuit());
        auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(suitPath);
        if (texture != nullptr)
        {
            _suitSprite->setTexture(texture);
            // 保持缩放比例
            float suitSize = _cardSize.width * 0.11f;
            _suitSprite->setScale(suitSize / _suitSprite->getContentSize().width);
        }
    }
    
    // 更新中央大数字
    if (_bigNumberSprite != nullptr)
    {
        std::string bigNumPath = resConfig->getBigNumberImagePath(_cardModel->getFace(), isRed);
        auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(bigNumPath);
        if (texture != nullptr)
        {
            _bigNumberSprite->setTexture(texture);
            // 保持缩放比例
            float bigSize = _cardSize.width * 0.38f;
            _bigNumberSprite->setScale(bigSize / _bigNumberSprite->getContentSize().width);
        }
    }
}


