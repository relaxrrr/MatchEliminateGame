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

#ifndef __GAME_RESULT_VIEW_H__
#define __GAME_RESULT_VIEW_H__

#include "cocos2d.h"

/**
 * @brief 游戏结果类型
 */
enum class GameResultType
{
    VICTORY,    // 闯关成功
    DEFEAT      // 闯关失败
};

/**
 * @brief 游戏结果弹窗视图
 * 显示游戏结束信息（成功/失败）和操作按钮（再来一局/退出）
 */
class GameResultView : public cocos2d::Layer
{
public:
    /**
     * @brief 创建游戏结果弹窗
     * @param resultType 游戏结果类型（成功/失败）
     * @return 游戏结果弹窗对象
     */
    static GameResultView* create(GameResultType resultType);
    
    /**
     * @brief 初始化
     * @param resultType 游戏结果类型
     * @return 是否成功
     */
    bool init(GameResultType resultType);
    
    /**
     * @brief 设置再来一局按钮回调
     * @param callback 回调函数
     */
    void setRestartCallback(const std::function<void()>& callback) { _restartCallback = callback; }
    
    /**
     * @brief 设置退出按钮回调
     * @param callback 回调函数
     */
    void setExitCallback(const std::function<void()>& callback) { _exitCallback = callback; }

private:
    /**
     * @brief 再来一局按钮点击处理
     */
    void onRestartButtonClicked(cocos2d::Ref* sender);
    
    /**
     * @brief 退出按钮点击处理
     */
    void onExitButtonClicked(cocos2d::Ref* sender);
    
    GameResultType _resultType;                          // 游戏结果类型
    std::function<void()> _restartCallback;             // 再来一局回调
    std::function<void()> _exitCallback;                // 退出回调
};

#endif // __GAME_RESULT_VIEW_H__

