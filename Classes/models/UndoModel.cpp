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

#include "UndoModel.h"

UndoModel::UndoModel()
: _actionType(UndoActionType::ELIMINATE_CARD)
, _cardId(0)
, _replacedCardId(0)
, _cardPosition(cocos2d::Vec2::ZERO)
, _replacedCardPosition(cocos2d::Vec2::ZERO)
, _cardModel(nullptr)
, _replacedCardModel(nullptr)
{
}

UndoModel::~UndoModel()
{
    if (_cardModel != nullptr)
    {
        delete _cardModel;
        _cardModel = nullptr;
    }
    if (_replacedCardModel != nullptr)
    {
        delete _replacedCardModel;
        _replacedCardModel = nullptr;
    }
}

cocos2d::ValueMap UndoModel::toValueMap() const
{
    cocos2d::ValueMap map;
    map["actionType"] = (int)_actionType;
    map["cardId"] = _cardId;
    map["replacedCardId"] = _replacedCardId;
    map["cardPositionX"] = _cardPosition.x;
    map["cardPositionY"] = _cardPosition.y;
    map["replacedCardPositionX"] = _replacedCardPosition.x;
    map["replacedCardPositionY"] = _replacedCardPosition.y;
    
    if (_cardModel != nullptr)
    {
        map["cardModel"] = _cardModel->toValueMap();
    }
    if (_replacedCardModel != nullptr)
    {
        map["replacedCardModel"] = _replacedCardModel->toValueMap();
    }
    
    return map;
}

bool UndoModel::fromValueMap(const cocos2d::ValueMap& map)
{
    if (map.find("actionType") == map.end() ||
        map.find("cardId") == map.end())
    {
        return false;
    }
    
    _actionType = (UndoActionType)map.at("actionType").asInt();
    _cardId = map.at("cardId").asInt();
    
    if (map.find("replacedCardId") != map.end())
    {
        _replacedCardId = map.at("replacedCardId").asInt();
    }
    
    if (map.find("cardPositionX") != map.end() && map.find("cardPositionY") != map.end())
    {
        _cardPosition.x = map.at("cardPositionX").asFloat();
        _cardPosition.y = map.at("cardPositionY").asFloat();
    }
    
    if (map.find("replacedCardPositionX") != map.end() && map.find("replacedCardPositionY") != map.end())
    {
        _replacedCardPosition.x = map.at("replacedCardPositionX").asFloat();
        _replacedCardPosition.y = map.at("replacedCardPositionY").asFloat();
    }
    
    if (map.find("cardModel") != map.end())
    {
        _cardModel = new CardModel();
        _cardModel->fromValueMap(map.at("cardModel").asValueMap());
    }
    
    if (map.find("replacedCardModel") != map.end())
    {
        _replacedCardModel = new CardModel();
        _replacedCardModel->fromValueMap(map.at("replacedCardModel").asValueMap());
    }
    
    return true;
}

