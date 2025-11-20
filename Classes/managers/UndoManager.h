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

#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "cocos2d.h"
#include "../models/UndoModel.h"
#include <vector>

// 前向声明
class GameModel;

/**
 * @brief 撤销功能管理器
 * 管理游戏操作的撤销记录，可持有model数据并对model数据进行加工
 * 禁止实现为单例模式，作为controller的成员变量
 */
class UndoManager
{
public:
    UndoManager();
    virtual ~UndoManager();
    
    /**
     * @brief 初始化
     * @param gameModel 游戏模型指针
     */
    void init(GameModel* gameModel);
    
    /**
     * @brief 添加撤销记录
     * @param undoModel 撤销数据模型
     */
    void pushUndoRecord(UndoModel* undoModel);
    
    /**
     * @brief 执行撤销操作
     * @return 撤销数据模型，如果没有可撤销的记录返回nullptr
     */
    UndoModel* popUndoRecord();
    
    /**
     * @brief 检查是否有可撤销的记录
     */
    bool canUndo() const { return !_undoStack.empty(); }
    
    /**
     * @brief 清空所有撤销记录
     */
    void clear();

private:
    GameModel* _gameModel;                      // 游戏模型指针
    std::vector<UndoModel*> _undoStack;         // 撤销记录栈
};

#endif // __UNDO_MANAGER_H__

