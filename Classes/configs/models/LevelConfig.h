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

#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "cocos2d.h"
#include <vector>

/**
 * @brief 关卡配置类
 * 存储关卡的静态配置信息
 */
class LevelConfig
{
public:
    LevelConfig();
    virtual ~LevelConfig();
    
    /**
     * @brief 获取关卡ID
     */
    int getLevelId() const { return _levelId; }
    void setLevelId(int levelId) { _levelId = levelId; }
    
    /**
     * @brief 获取关卡名称
     */
    std::string getLevelName() const { return _levelName; }
    void setLevelName(const std::string& name) { _levelName = name; }
    
    /**
     * @brief 获取主牌区的初始卡牌配置
     * @return 卡牌ID列表
     */
    std::vector<int> getPlayFieldCards() const { return _playFieldCards; }
    void setPlayFieldCards(const std::vector<int>& cards) { _playFieldCards = cards; }
    
    /**
     * @brief 获取手牌区的初始卡牌配置
     * @return 卡牌ID列表
     */
    std::vector<int> getStackCards() const { return _stackCards; }
    void setStackCards(const std::vector<int>& cards) { _stackCards = cards; }

private:
    int _levelId;
    std::string _levelName;
    std::vector<int> _playFieldCards;  // 主牌区卡牌ID列表
    std::vector<int> _stackCards;      // 手牌区卡牌ID列表
};

#endif // __LEVEL_CONFIG_H__

