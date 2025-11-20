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

#ifndef __STACK_CONTROLLER_H__
#define __STACK_CONTROLLER_H__

#include "cocos2d.h"

// 前向声明
class GameModel;
class StackView;
class UndoManager;

/**
 * @brief 手牌区控制器
 * 处理手牌区相关的业务逻辑
 */
class StackController
{
public:
    StackController();
    virtual ~StackController();
    
    /**
     * @brief 初始化
     * @param gameModel 游戏模型
     * @param undoManager 撤销管理器
     * @param bottomCardView 底牌视图
     */
    void init(GameModel* gameModel, UndoManager* undoManager, class BottomCardView* bottomCardView);
    
    /**
     * @brief 初始化视图
     * @param stackView 备用牌堆视图
     */
    void initView(StackView* stackView);
    
    /**
     * @brief 处理备用牌堆卡牌点击（点击顶部牌，替换底牌）
     * @param cardId 被点击的卡牌ID
     * @return 是否处理成功
     */
    bool handleCardClick(int cardId);
    
    /**
     * @brief 执行撤销操作
     * @param undoModel 撤销数据模型
     * @return 是否成功
     */
    bool performUndo(class UndoModel* undoModel);
    
    /**
     * @brief 设置游戏状态检查回调
     * @param callback 回调函数（在操作完成后调用）
     */
    void setGameStateCheckCallback(const std::function<void()>& callback) { _gameStateCheckCallback = callback; }

private:
    GameModel* _gameModel;              // 游戏模型
    StackView* _stackView;              // 备用牌堆视图
    class BottomCardView* _bottomCardView; // 底牌视图
    UndoManager* _undoManager;          // 撤销管理器
    std::function<void()> _gameStateCheckCallback;  // 游戏状态检查回调
};

#endif // __STACK_CONTROLLER_H__

