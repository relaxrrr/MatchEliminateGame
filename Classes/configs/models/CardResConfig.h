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

#ifndef __CARD_RES_CONFIG_H__
#define __CARD_RES_CONFIG_H__

#include "cocos2d.h"

/**
 * @brief 花色类型枚举
 */
enum CardSuitType
{
    CST_NONE = -1,
    CST_CLUBS,      // 梅花
    CST_DIAMONDS,   // 方块
    CST_HEARTS,     // 红桃
    CST_SPADES,     // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

/**
 * @brief 卡牌点数类型枚举
 */
enum CardFaceType
{
    CFT_NONE = -1,
    CFT_ACE,        // A
    CFT_TWO,        // 2
    CFT_THREE,      // 3
    CFT_FOUR,       // 4
    CFT_FIVE,       // 5
    CFT_SIX,        // 6
    CFT_SEVEN,      // 7
    CFT_EIGHT,      // 8
    CFT_NINE,       // 9
    CFT_TEN,        // 10
    CFT_JACK,       // J
    CFT_QUEEN,      // Q
    CFT_KING,       // K
    CFT_NUM_CARD_FACE_TYPES
};

/**
 * @brief 卡牌UI资源配置类
 * 负责管理卡牌的图片资源路径等配置信息
 */
class CardResConfig
{
public:
    /**
     * @brief 获取单例实例
     */
    static CardResConfig* getInstance();
    
    /**
     * @brief 销毁单例实例
     */
    static void destroyInstance();
    
    /**
     * @brief 获取卡牌背景图片资源路径
     * @return 资源路径
     */
    std::string getCardBackgroundPath();
    
    /**
     * @brief 获取花色图标资源路径
     * @param suit 花色
     * @return 资源路径
     */
    std::string getSuitImagePath(CardSuitType suit);
    
    /**
     * @brief 获取大数字图标资源路径
     * @param face 点数
     * @param isRed 是否为红色（红桃、方块为红色，梅花、黑桃为黑色）
     * @return 资源路径
     */
    std::string getBigNumberImagePath(CardFaceType face, bool isRed);
    
    /**
     * @brief 获取小数字图标资源路径
     * @param face 点数
     * @param isRed 是否为红色（红桃、方块为红色，梅花、黑桃为黑色）
     * @return 资源路径
     */
    std::string getSmallNumberImagePath(CardFaceType face, bool isRed);
    
    /**
     * @brief 判断花色是否为红色
     * @param suit 花色
     * @return true表示红色，false表示黑色
     */
    bool isRedSuit(CardSuitType suit);
    
    /**
     * @brief 将点数转换为字符串（用于文件名）
     * @param face 点数
     * @return 字符串（如"A", "2", "3", ..., "K"）
     */
    std::string faceToString(CardFaceType face);
    
    /**
     * @brief 获取卡牌背面图片资源路径
     * @return 资源路径
     */
    std::string getCardBackImagePath();

private:
    CardResConfig();
    virtual ~CardResConfig();
    
    static CardResConfig* _instance;
};

#endif // __CARD_RES_CONFIG_H__

