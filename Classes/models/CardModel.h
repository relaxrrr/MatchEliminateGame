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

#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"
#include "../configs/models/CardResConfig.h"

/**
 * @brief 卡牌数据模型
 * 存储卡牌的核心数据，支持序列化和反序列化
 */
class CardModel
{
public:
    CardModel();
    CardModel(int cardId, CardSuitType suit, CardFaceType face);
    virtual ~CardModel();
    
    /**
     * @brief 获取卡牌ID（唯一标识）
     */
    int getCardId() const { return _cardId; }
    void setCardId(int cardId) { _cardId = cardId; }
    
    /**
     * @brief 获取花色
     */
    CardSuitType getSuit() const { return _suit; }
    void setSuit(CardSuitType suit) { _suit = suit; }
    
    /**
     * @brief 获取点数
     */
    CardFaceType getFace() const { return _face; }
    void setFace(CardFaceType face) { _face = face; }
    
    /**
     * @brief 获取点数数值（用于匹配判断）
     * @return 点数数值，A=1, 2=2, ..., K=13
     */
    int getFaceValue() const;
    
    /**
     * @brief 判断两张卡牌点数是否相差1
     * @param other 另一张卡牌
     * @return true表示点数差1，可以匹配
     */
    bool canMatchWith(const CardModel* other) const;
    
    /**
     * @brief 序列化（用于存档）
     */
    cocos2d::ValueMap toValueMap() const;
    
    /**
     * @brief 反序列化（用于读档）
     */
    bool fromValueMap(const cocos2d::ValueMap& map);

private:
    int _cardId;            // 卡牌唯一ID
    CardSuitType _suit;     // 花色
    CardFaceType _face;     // 点数
};

#endif // __CARD_MODEL_H__

