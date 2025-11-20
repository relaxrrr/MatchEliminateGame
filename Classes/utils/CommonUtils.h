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

#ifndef __COMMON_UTILS_H__
#define __COMMON_UTILS_H__

#include "cocos2d.h"
#include "../configs/models/CardResConfig.h"

/**
 * @brief 通用工具类
 * 提供独立于业务逻辑的通用功能
 */
class CommonUtils
{
public:
    /**
     * @brief 生成随机整数
     * @param min 最小值
     * @param max 最大值
     * @return 随机数
     */
    static int randomInt(int min, int max);
    
    /**
     * @brief 打乱数组顺序
     * @param vec 要打乱的数组
     */
    template<typename T>
    static void shuffleVector(std::vector<T>& vec);
    
    /**
     * @brief 深拷贝CardModel
     * @param src 源卡牌
     * @return 新的卡牌对象
     */
    static class CardModel* cloneCardModel(const class CardModel* src);
};

template<typename T>
void CommonUtils::shuffleVector(std::vector<T>& vec)
{
    for (size_t i = vec.size() - 1; i > 0; --i)
    {
        int j = randomInt(0, (int)i);
        std::swap(vec[i], vec[j]);
    }
}

#endif // __COMMON_UTILS_H__

