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

#include "CardModel.h"

CardModel::CardModel()
: _cardId(0)
, _suit(CST_NONE)
, _face(CFT_NONE)
{
}

CardModel::CardModel(int cardId, CardSuitType suit, CardFaceType face)
: _cardId(cardId)
, _suit(suit)
, _face(face)
{
}

CardModel::~CardModel()
{
}

int CardModel::getFaceValue() const
{
    if (_face == CFT_NONE)
    {
        return 0;
    }
    return (int)_face + 1;  // CFT_ACE=0 -> 1, CFT_TWO=1 -> 2, ...
}

bool CardModel::canMatchWith(const CardModel* other) const
{
    if (other == nullptr)
    {
        return false;
    }
    
    int thisValue = getFaceValue();
    int otherValue = other->getFaceValue();
    
    // 点数差1即可匹配（支持循环：K(13)和A(1)可以匹配，A(1)和K(13)也可以匹配）
    int diff = abs(thisValue - otherValue);
    if (diff == 1)
    {
        return true;  // 相邻点数
    }
    
    // 循环匹配：K(13) 和 A(1) 可以匹配
    if ((thisValue == 1 && otherValue == 13) || (thisValue == 13 && otherValue == 1))
    {
        return true;
    }
    
    return false;
}

cocos2d::ValueMap CardModel::toValueMap() const
{
    cocos2d::ValueMap map;
    map["cardId"] = _cardId;
    map["suit"] = (int)_suit;
    map["face"] = (int)_face;
    return map;
}

bool CardModel::fromValueMap(const cocos2d::ValueMap& map)
{
    if (map.find("cardId") == map.end() ||
        map.find("suit") == map.end() ||
        map.find("face") == map.end())
    {
        return false;
    }
    
    _cardId = map.at("cardId").asInt();
    _suit = (CardSuitType)map.at("suit").asInt();
    _face = (CardFaceType)map.at("face").asInt();
    
    return true;
}

