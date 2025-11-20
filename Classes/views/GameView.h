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

#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include "PlayFieldView.h"
#include "StackView.h"
#include "BottomCardView.h"
#include "GameResultView.h"

// 前向声明
class GameModel;

/**
 * @brief 游戏主视图
 * 承载所有UI元素，包括主牌堆、备用牌堆、底牌、回退按钮等
 */
class GameView : public cocos2d::Scene
{
public:
    /**
     * @brief 创建游戏视图
     * @param gameModel 游戏模型（const指针）
     * @return 游戏视图对象
     */
    static GameView* create(const GameModel* gameModel);
    
    /**
     * @brief 初始化
     * @param gameModel 游戏模型
     * @return 是否成功
     */
    bool init(const GameModel* gameModel);
    
    /**
     * @brief 获取主牌堆视图
     */
    PlayFieldView* getPlayFieldView() const { return _playFieldView; }
    
    /**
     * @brief 获取备用牌堆视图
     */
    StackView* getStackView() const { return _stackView; }
    
    /**
     * @brief 获取底牌视图
     */
    BottomCardView* getBottomCardView() const { return _bottomCardView; }
    
    /**
     * @brief 更新显示（根据model更新UI）
     * @param gameModel 游戏模型
     */
    void updateView(const GameModel* gameModel);
    
    /**
     * @brief 设置回退按钮点击回调
     * @param callback 回调函数
     */
    void setUndoButtonCallback(const std::function<void()>& callback);
    
    /**
     * @brief 显示游戏结果弹窗
     * @param resultType 游戏结果类型（成功/失败）
     */
    void showGameResult(GameResultType resultType);
    
    /**
     * @brief 设置重新开始回调
     * @param callback 回调函数
     */
    void setRestartCallback(const std::function<void()>& callback) { _restartCallback = callback; }
    
    /**
     * @brief 设置退出回调
     * @param callback 回调函数
     */
    void setExitCallback(const std::function<void()>& callback) { _exitCallback = callback; }
    
    /**
     * @brief 移除游戏结果弹窗
     */
    void removeGameResultView();

private:
    /**
     * @brief 初始化UI布局
     */
    void initUI();
    
    /**
     * @brief 回退按钮点击处理
     */
    void onUndoButtonClicked(cocos2d::Ref* sender);
    
    const GameModel* _gameModel;                     // 游戏模型（const指针）
    PlayFieldView* _playFieldView;                    // 主牌堆视图
    StackView* _stackView;                            // 备用牌堆视图
    BottomCardView* _bottomCardView;                  // 底牌视图
    cocos2d::MenuItemLabel* _undoButton;              // 回退按钮
    std::function<void()> _undoButtonCallback;        // 回退按钮回调
    std::function<void()> _restartCallback;           // 重新开始回调
    std::function<void()> _exitCallback;              // 退出回调
    GameResultView* _gameResultView;                  // 游戏结果弹窗
};

#endif // __GAME_VIEW_H__

