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

#include "LevelConfigLoader.h"

LevelConfig* LevelConfigLoader::loadLevelConfig(int levelId)
{
    // TODO: 从配置文件加载
    // 目前返回默认配置
    return createDefaultLevelConfig();
}

LevelConfig* LevelConfigLoader::createDefaultLevelConfig()
{
    LevelConfig* config = new LevelConfig();
    config->setLevelId(1);
    config->setLevelName("Level 1");
    
    // 设置默认卡牌配置（示例）
    std::vector<int> playFieldCards;
    std::vector<int> stackCards;
    
    // TODO: 根据实际需求设置卡牌
    config->setPlayFieldCards(playFieldCards);
    config->setStackCards(stackCards);
    
    return config;
}

