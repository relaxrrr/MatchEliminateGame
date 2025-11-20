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

#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "cocos2d.h"
#include "CardModel.h"
#include <vector>
#include <map>

/**
 * @brief 游戏数据模型
 * 存储游戏运行时的所有动态数据
 */
class GameModel
{
public:
    GameModel();
    virtual ~GameModel();
    
    /**
     * @brief 获取关卡ID
     */
    int getLevelId() const { return _levelId; }
    void setLevelId(int levelId) { _levelId = levelId; }
    
    /**
     * @brief 获取所有卡牌数据（通过ID访问）
     */
    std::map<int, CardModel*>& getAllCards() { return _allCards; }
    const std::map<int, CardModel*>& getAllCards() const { return _allCards; }
    
    /**
     * @brief 根据ID获取卡牌
     */
    CardModel* getCardById(int cardId);
    const CardModel* getCardById(int cardId) const;
    
    /**
     * @brief 添加卡牌
     */
    void addCard(CardModel* card);
    
    /**
     * @brief 获取主牌堆的卡牌ID列表
     */
    std::vector<int>& getPlayFieldCardIds() { return _playFieldCardIds; }
    const std::vector<int>& getPlayFieldCardIds() const { return _playFieldCardIds; }
    
    /**
     * @brief 获取备用牌堆的卡牌ID列表（从底部到顶部）
     */
    std::vector<int>& getStackCardIds() { return _stackCardIds; }
    const std::vector<int>& getStackCardIds() const { return _stackCardIds; }
    
    /**
     * @brief 获取底牌ID（只有一张）
     */
    int getBottomCardId() const { return _bottomCardId; }
    void setBottomCardId(int cardId) { _bottomCardId = cardId; }
    
    /**
     * @brief 从主牌堆移除卡牌
     * @param cardId 卡牌ID
     * @return 是否成功移除
     */
    bool removeFromPlayField(int cardId);
    
    /**
     * @brief 从备用牌堆移除顶部卡牌
     * @return 被移除的卡牌ID，如果备用牌堆为空返回0
     */
    int removeTopFromStack();
    
    /**
     * @brief 将卡牌添加到备用牌堆顶部
     * @param cardId 卡牌ID
     */
    void addToStackTop(int cardId);
    
    /**
     * @brief 序列化
     */
    cocos2d::ValueMap toValueMap() const;
    
    /**
     * @brief 反序列化
     */
    bool fromValueMap(const cocos2d::ValueMap& map);
    
    /**
     * @brief 清理所有数据
     */
    void clear();

private:
    int _levelId;
    std::map<int, CardModel*> _allCards;        // 所有卡牌（cardId -> CardModel）
    std::vector<int> _playFieldCardIds;         // 主牌堆卡牌ID列表
    std::vector<int> _stackCardIds;            // 备用牌堆卡牌ID列表（从底部到顶部）
    int _bottomCardId;                          // 底牌ID（只有一张）
};

#endif // __GAME_MODEL_H__

