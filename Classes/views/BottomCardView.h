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

#ifndef __BOTTOM_CARD_VIEW_H__
#define __BOTTOM_CARD_VIEW_H__

#include "cocos2d.h"
#include "CardView.h"

// 前向声明
class GameModel;

/**
 * @brief 底牌视图
 * 显示底牌（只有一张）
 */
class BottomCardView : public cocos2d::Node
{
public:
    /**
     * @brief 创建底牌视图
     * @param gameModel 游戏模型（const指针）
     * @return 底牌视图对象
     */
    static BottomCardView* create(const GameModel* gameModel);
    
    /**
     * @brief 初始化
     * @param gameModel 游戏模型
     * @return 是否成功
     */
    bool init(const GameModel* gameModel);
    
    /**
     * @brief 更新显示（根据model更新UI）
     * @param gameModel 游戏模型
     */
    void updateView(const GameModel* gameModel);
    
    /**
     * @brief 播放卡牌替换动画
     * @param cardId 新卡牌ID
     * @param duration 动画时长
     * @param callback 完成回调
     */
    void playReplaceAnimation(int cardId, float duration, const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 播放回退动画
     * @param cardId 恢复的卡牌ID
     * @param duration 动画时长
     * @param callback 完成回调
     */
    void playUndoAnimation(int cardId, float duration, const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 获取卡牌视图
     */
    CardView* getCardView() const { return _cardView; }

private:
    const GameModel* _gameModel;                     // 游戏模型（const指针）
    CardView* _cardView;                              // 底牌视图
    cocos2d::Size _cardSize;                          // 卡牌尺寸
};

#endif // __BOTTOM_CARD_VIEW_H__

