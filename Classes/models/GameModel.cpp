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

#include "GameModel.h"

GameModel::GameModel()
: _levelId(0)
, _bottomCardId(0)
{
}

GameModel::~GameModel()
{
    clear();
}

CardModel* GameModel::getCardById(int cardId)
{
    auto it = _allCards.find(cardId);
    if (it != _allCards.end())
    {
        return it->second;
    }
    return nullptr;
}

const CardModel* GameModel::getCardById(int cardId) const
{
    auto it = _allCards.find(cardId);
    if (it != _allCards.end())
    {
        return it->second;
    }
    return nullptr;
}

void GameModel::addCard(CardModel* card)
{
    if (card != nullptr)
    {
        _allCards[card->getCardId()] = card;
    }
}

bool GameModel::removeFromPlayField(int cardId)
{
    auto it = std::find(_playFieldCardIds.begin(), _playFieldCardIds.end(), cardId);
    if (it != _playFieldCardIds.end())
    {
        _playFieldCardIds.erase(it);
        return true;
    }
    return false;
}

int GameModel::removeTopFromStack()
{
    if (_stackCardIds.empty())
    {
        return 0;
    }
    int topCardId = _stackCardIds.back();
    _stackCardIds.pop_back();
    return topCardId;
}

void GameModel::addToStackTop(int cardId)
{
    _stackCardIds.push_back(cardId);
}

cocos2d::ValueMap GameModel::toValueMap() const
{
    cocos2d::ValueMap map;
    map["levelId"] = _levelId;
    
    cocos2d::ValueMap cardsMap;
    for (auto& pair : _allCards)
    {
        cardsMap[std::to_string(pair.first)] = pair.second->toValueMap();
    }
    map["allCards"] = cardsMap;
    
    cocos2d::ValueVector playFieldVec;
    for (int cardId : _playFieldCardIds)
    {
        playFieldVec.push_back(cocos2d::Value(cardId));
    }
    map["playFieldCardIds"] = playFieldVec;
    
    cocos2d::ValueVector stackVec;
    for (int cardId : _stackCardIds)
    {
        stackVec.push_back(cocos2d::Value(cardId));
    }
    map["stackCardIds"] = stackVec;
    map["bottomCardId"] = _bottomCardId;
    
    return map;
}

bool GameModel::fromValueMap(const cocos2d::ValueMap& map)
{
    if (map.find("levelId") == map.end())
    {
        return false;
    }
    
    clear();
    
    _levelId = map.at("levelId").asInt();
    
    if (map.find("allCards") != map.end())
    {
        cocos2d::ValueMap cardsMap = map.at("allCards").asValueMap();
        for (auto& pair : cardsMap)
        {
            CardModel* card = new CardModel();
            if (card->fromValueMap(pair.second.asValueMap()))
            {
                _allCards[card->getCardId()] = card;
            }
            else
            {
                delete card;
            }
        }
    }
    
    if (map.find("playFieldCardIds") != map.end())
    {
        cocos2d::ValueVector vec = map.at("playFieldCardIds").asValueVector();
        for (auto& value : vec)
        {
            _playFieldCardIds.push_back(value.asInt());
        }
    }
    
    if (map.find("stackCardIds") != map.end())
    {
        cocos2d::ValueVector vec = map.at("stackCardIds").asValueVector();
        for (auto& value : vec)
        {
            _stackCardIds.push_back(value.asInt());
        }
    }
    
    if (map.find("bottomCardId") != map.end())
    {
        _bottomCardId = map.at("bottomCardId").asInt();
    }
    
    return true;
}

void GameModel::clear()
{
    for (auto& pair : _allCards)
    {
        delete pair.second;
    }
    _allCards.clear();
    _playFieldCardIds.clear();
    _stackCardIds.clear();
    _bottomCardId = 0;
}

