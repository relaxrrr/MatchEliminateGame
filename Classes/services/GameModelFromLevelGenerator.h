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

#ifndef __GAME_MODEL_FROM_LEVEL_GENERATOR_H__
#define __GAME_MODEL_FROM_LEVEL_GENERATOR_H__

#include "cocos2d.h"

// 前向声明
class LevelConfig;
class GameModel;

/**
 * @brief 游戏模型生成服务
 * 将静态配置（LevelConfig）转换为动态运行时数据（GameModel）
 * 处理卡牌随机生成策略等
 */
class GameModelFromLevelGenerator
{
public:
    /**
     * @brief 从关卡配置生成游戏模型
     * @param levelConfig 关卡配置
     * @return 游戏模型对象，失败返回nullptr
     */
    static GameModel* generateGameModel(const LevelConfig* levelConfig);
    
    /**
     * @brief 生成随机卡牌列表
     * @param count 卡牌数量
     * @param startCardId 起始卡牌ID
     * @return 卡牌ID列表
     */
    static std::vector<int> generateRandomCards(int count, int startCardId = 1);

private:
    GameModelFromLevelGenerator() {}
    virtual ~GameModelFromLevelGenerator() {}
};

#endif // __GAME_MODEL_FROM_LEVEL_GENERATOR_H__

