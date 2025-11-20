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

#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"

// 前向声明
class CardModel;

/**
 * @brief 卡牌视图
 * 负责卡牌的UI显示，可持有const类型的model指针
 * 卡片由背景、左上角小数字、右上角花色、中央大数字组成
 */
class CardView : public cocos2d::Node
{
public:
    /**
     * @brief 创建卡牌视图
     * @param cardModel 卡牌数据模型（const指针）
     * @return 卡牌视图对象
     */
    static CardView* create(const CardModel* cardModel);
    
    /**
     * @brief 初始化
     * @param cardModel 卡牌数据模型
     * @return 是否成功
     */
    bool init(const CardModel* cardModel);
    
    /**
     * @brief 获取卡牌ID
     */
    int getCardId() const { return _cardId; }
    
    /**
     * @brief 更新卡牌显示（根据model更新UI）
     * @param cardModel 卡牌数据模型
     */
    void updateCard(const CardModel* cardModel);
    
    /**
     * @brief 播放平移动画
     * @param targetPos 目标位置
     * @param duration 动画时长
     * @param callback 完成回调
     */
    void playMoveAnimation(const cocos2d::Vec2& targetPos, float duration, 
                          const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 获取卡牌尺寸
     */
    cocos2d::Size getCardSize() const { return _cardSize; }

private:
    /**
     * @brief 创建卡牌UI元素
     */
    void createCardElements();
    
    /**
     * @brief 更新卡牌UI元素
     */
    void updateCardElements();

private:
    int _cardId;                        // 卡牌ID
    const CardModel* _cardModel;        // 卡牌数据模型（const指针）
    cocos2d::Sprite* _backgroundSprite; // 背景精灵
    cocos2d::Sprite* _smallNumberSprite; // 左上角小数字精灵
    cocos2d::Sprite* _suitSprite;       // 左上角花色精灵
    cocos2d::Sprite* _bigNumberSprite;  // 中央大数字精灵
    cocos2d::Size _cardSize;            // 卡牌尺寸
};

#endif // __CARD_VIEW_H__

