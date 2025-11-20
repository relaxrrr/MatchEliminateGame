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

#include "GameModelFromLevelGenerator.h"
#include "../configs/models/LevelConfig.h"
#include "../models/GameModel.h"
#include "../models/CardModel.h"
#include "../utils/CommonUtils.h"

GameModel* GameModelFromLevelGenerator::generateGameModel(const LevelConfig* levelConfig)
{
    if (levelConfig == nullptr)
    {
        return nullptr;
    }
    
    GameModel* gameModel = new GameModel();
    gameModel->setLevelId(levelConfig->getLevelId());
    
    int cardIdCounter = 1;
    
    // 生成主牌区卡牌（8张牌）
    std::vector<int> playFieldCardIds = levelConfig->getPlayFieldCards();
    if (playFieldCardIds.empty())
    {
        // 如果没有配置，生成8张随机卡牌
        playFieldCardIds = generateRandomCards(8, cardIdCounter);
        cardIdCounter += 8;
    }
    
    for (int cardId : playFieldCardIds)
    {
        // 生成随机卡牌数据（示例）
        CardSuitType suit = (CardSuitType)CommonUtils::randomInt(0, CST_NUM_CARD_SUIT_TYPES - 1);
        CardFaceType face = (CardFaceType)CommonUtils::randomInt(0, CFT_NUM_CARD_FACE_TYPES - 1);
        
        CardModel* card = new CardModel(cardId, suit, face);
        gameModel->addCard(card);
        gameModel->getPlayFieldCardIds().push_back(cardId);
    }
    
    // 生成备用牌堆卡牌
    std::vector<int> stackCardIds = levelConfig->getStackCards();
    if (stackCardIds.empty())
    {
        // 如果没有配置，生成12张随机卡牌
        stackCardIds = generateRandomCards(12, cardIdCounter);
        cardIdCounter += 12;
    }
    
    for (int cardId : stackCardIds)
    {
        // 生成随机卡牌数据（示例）
        CardSuitType suit = (CardSuitType)CommonUtils::randomInt(0, CST_NUM_CARD_SUIT_TYPES - 1);
        CardFaceType face = (CardFaceType)CommonUtils::randomInt(0, CFT_NUM_CARD_FACE_TYPES - 1);
        
        CardModel* card = new CardModel(cardId, suit, face);
        gameModel->addCard(card);
        gameModel->getStackCardIds().push_back(cardId);
    }
    
    // 生成底牌（从备用牌堆顶部取一张，如果没有则生成一张）
    if (!stackCardIds.empty())
    {
        int bottomCardId = stackCardIds.back();
        gameModel->getStackCardIds().pop_back();
        gameModel->setBottomCardId(bottomCardId);
    }
    else
    {
        // 如果没有备用牌堆，生成一张底牌
        int bottomCardId = cardIdCounter++;
        CardSuitType suit = (CardSuitType)CommonUtils::randomInt(0, CST_NUM_CARD_SUIT_TYPES - 1);
        CardFaceType face = (CardFaceType)CommonUtils::randomInt(0, CFT_NUM_CARD_FACE_TYPES - 1);
        CardModel* bottomCard = new CardModel(bottomCardId, suit, face);
        gameModel->addCard(bottomCard);
        gameModel->setBottomCardId(bottomCardId);
    }
    
    return gameModel;
}

std::vector<int> GameModelFromLevelGenerator::generateRandomCards(int count, int startCardId)
{
    std::vector<int> cardIds;
    for (int i = 0; i < count; ++i)
    {
        cardIds.push_back(startCardId + i);
    }
    CommonUtils::shuffleVector(cardIds);
    return cardIds;
}

