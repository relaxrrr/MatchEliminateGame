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

#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"

// 前向声明
class GameModel;
class GameView;
class PlayFieldController;
class StackController;
class UndoManager;

/**
 * @brief 游戏主控制器
 * 管理整个游戏流程，协调各个子控制器
 */
class GameController
{
public:
    GameController();
    virtual ~GameController();
    
    /**
     * @brief 开始游戏
     * @param levelId 关卡ID
     * @return 游戏视图场景
     */
    cocos2d::Scene* startGame(int levelId);
    
    /**
     * @brief 处理撤销操作（需求3：回退功能）
     */
    void handleUndo();
    
    /**
     * @brief 检查游戏状态（胜利/失败）
     * 在每次操作后调用，检查是否满足游戏结束条件
     */
    void checkGameState();
    
    /**
     * @brief 重新开始游戏
     */
    void restartGame();
    
    /**
     * @brief 退出游戏
     */
    void exitGame();

private:
    /**
     * @brief 初始化子控制器
     */
    void initControllers();
    
    GameModel* _gameModel;                    // 游戏模型
    GameView* _gameView;                      // 游戏视图
    PlayFieldController* _playFieldController; // 主牌区控制器
    StackController* _stackController;         // 手牌区控制器
    UndoManager* _undoManager;                // 撤销管理器
    int _levelId;                             // 当前关卡ID
};

#endif // __GAME_CONTROLLER_H__

