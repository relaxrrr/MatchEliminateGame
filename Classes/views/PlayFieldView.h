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

#ifndef __PLAY_FIELD_VIEW_H__
#define __PLAY_FIELD_VIEW_H__

#include "cocos2d.h"
#include "CardView.h"
#include <vector>
#include <map>

// 前向声明
class GameModel;

/**
 * @brief 主牌区视图
 * 负责主牌区的UI显示和用户交互
 */
class PlayFieldView : public cocos2d::Layer
{
public:
    /**
     * @brief 卡牌点击回调函数类型
     */
    typedef std::function<void(int cardId)> CardClickCallback;
    
    /**
     * @brief 创建主牌区视图
     * @param gameModel 游戏模型（const指针）
     * @return 主牌区视图对象
     */
    static PlayFieldView* create(const GameModel* gameModel);
    
    /**
     * @brief 初始化
     * @param gameModel 游戏模型
     * @return 是否成功
     */
    bool init(const GameModel* gameModel);
    
    /**
     * @brief 设置卡牌点击回调
     * @param callback 回调函数
     */
    void setCardClickCallback(const CardClickCallback& callback) { _cardClickCallback = callback; }
    
    /**
     * @brief 更新显示（根据model更新UI）
     * @param gameModel 游戏模型
     */
    void updateView(const GameModel* gameModel);
    
    /**
     * @brief 播放卡牌平移动画（用于匹配替换）
     * @param cardId 卡牌ID
     * @param targetPos 目标位置
     * @param duration 动画时长
     * @param callback 完成回调
     */
    void playCardMoveAnimation(int cardId, const cocos2d::Vec2& targetPos, 
                              float duration, const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 播放回退动画
     * @param cardId 卡牌ID
     * @param targetPos 目标位置（原位置）
     * @param duration 动画时长
     * @param callback 完成回调
     */
    void playUndoAnimation(int cardId, const cocos2d::Vec2& targetPos, 
                          float duration, const std::function<void()>& callback = nullptr);
    
    /**
     * @brief 获取卡牌视图
     * @param cardId 卡牌ID
     * @return 卡牌视图，不存在返回nullptr
     */
    CardView* getCardView(int cardId);

private:
    /**
     * @brief 处理触摸事件
     */
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    /**
     * @brief 获取卡牌位置
     * @param index 卡牌在主牌堆中的索引
     * @return 卡牌位置
     */
    cocos2d::Vec2 getCardPosition(int index);
    
    const GameModel* _gameModel;                     // 游戏模型（const指针）
    std::map<int, CardView*> _cardViews;              // 卡牌视图映射（cardId -> CardView）
    std::map<int, int> _cardOriginalIndex;            // 卡牌原始索引映射（cardId -> originalIndex）
    CardClickCallback _cardClickCallback;            // 卡牌点击回调
    cocos2d::Size _cardSize;                          // 卡牌尺寸
    float _cardSpacingX;                               // 卡牌横向间距
    float _cardSpacingY;                               // 卡牌纵向间距
};

#endif // __PLAY_FIELD_VIEW_H__

