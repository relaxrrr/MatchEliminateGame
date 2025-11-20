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

#include "CardResConfig.h"

CardResConfig* CardResConfig::_instance = nullptr;

CardResConfig* CardResConfig::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new CardResConfig();
    }
    return _instance;
}

void CardResConfig::destroyInstance()
{
    if (_instance != nullptr)
    {
        delete _instance;
        _instance = nullptr;
    }
}

CardResConfig::CardResConfig()
{
}

CardResConfig::~CardResConfig()
{
}

std::string CardResConfig::getCardBackgroundPath()
{
    return "res/card_general.png";
}

std::string CardResConfig::getSuitImagePath(CardSuitType suit)
{
    switch (suit)
    {
        case CST_CLUBS:
            return "res/suits/club.png";
        case CST_DIAMONDS:
            return "res/suits/diamond.png";
        case CST_HEARTS:
            return "res/suits/heart.png";
        case CST_SPADES:
            return "res/suits/spade.png";
        default:
            return "";
    }
}

std::string CardResConfig::getBigNumberImagePath(CardFaceType face, bool isRed)
{
    std::string color = isRed ? "red" : "black";
    std::string faceStr = faceToString(face);
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "res/number/big_%s_%s.png", color.c_str(), faceStr.c_str());
    return std::string(buffer);
}

std::string CardResConfig::getSmallNumberImagePath(CardFaceType face, bool isRed)
{
    std::string color = isRed ? "red" : "black";
    std::string faceStr = faceToString(face);
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "res/number/small_%s_%s.png", color.c_str(), faceStr.c_str());
    return std::string(buffer);
}

bool CardResConfig::isRedSuit(CardSuitType suit)
{
    return suit == CST_HEARTS || suit == CST_DIAMONDS;
}

std::string CardResConfig::faceToString(CardFaceType face)
{
    switch (face)
    {
        case CFT_ACE:
            return "A";
        case CFT_TWO:
            return "2";
        case CFT_THREE:
            return "3";
        case CFT_FOUR:
            return "4";
        case CFT_FIVE:
            return "5";
        case CFT_SIX:
            return "6";
        case CFT_SEVEN:
            return "7";
        case CFT_EIGHT:
            return "8";
        case CFT_NINE:
            return "9";
        case CFT_TEN:
            return "10";
        case CFT_JACK:
            return "J";
        case CFT_QUEEN:
            return "Q";
        case CFT_KING:
            return "K";
        default:
            return "";
    }
}

std::string CardResConfig::getCardBackImagePath()
{
    return "res/card_back.png";
}

