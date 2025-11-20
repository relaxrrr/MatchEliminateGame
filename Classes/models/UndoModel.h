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

#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include "cocos2d.h"
#include "CardModel.h"

/**
 * @brief 操作类型枚举
 */
enum class UndoActionType
{
    ELIMINATE_CARD,            // 消除主牌堆的卡牌
    REPLACE_BOTTOM_CARD        // 替换底牌
};

/**
 * @brief 回退数据模型
 * 存储单次操作的撤销信息
 */
class UndoModel
{
public:
    UndoModel();
    virtual ~UndoModel();
    
    /**
     * @brief 获取操作类型
     */
    UndoActionType getActionType() const { return _actionType; }
    void setActionType(UndoActionType type) { _actionType = type; }
    
    /**
     * @brief 获取被操作的卡牌ID
     * 消除操作：被消除的卡牌ID
     * 替换操作：被替换的底牌ID
     */
    int getCardId() const { return _cardId; }
    void setCardId(int cardId) { _cardId = cardId; }
    
    /**
     * @brief 获取替换的卡牌ID（仅用于替换操作）
     */
    int getReplacedCardId() const { return _replacedCardId; }
    void setReplacedCardId(int cardId) { _replacedCardId = cardId; }
    
    /**
     * @brief 获取卡牌位置（用于回退动画）
     */
    cocos2d::Vec2 getCardPosition() const { return _cardPosition; }
    void setCardPosition(const cocos2d::Vec2& pos) { _cardPosition = pos; }
    
    /**
     * @brief 获取被替换卡牌位置（仅用于替换操作，用于回退动画）
     */
    cocos2d::Vec2 getReplacedCardPosition() const { return _replacedCardPosition; }
    void setReplacedCardPosition(const cocos2d::Vec2& pos) { _replacedCardPosition = pos; }
    
    /**
     * @brief 获取卡牌数据（用于恢复）
     */
    CardModel* getCardModel() const { return _cardModel; }
    void setCardModel(CardModel* model) { _cardModel = model; }
    
    /**
     * @brief 获取被替换的卡牌数据（仅用于替换操作）
     */
    CardModel* getReplacedCardModel() const { return _replacedCardModel; }
    void setReplacedCardModel(CardModel* model) { _replacedCardModel = model; }
    
    /**
     * @brief 序列化
     */
    cocos2d::ValueMap toValueMap() const;
    
    /**
     * @brief 反序列化
     */
    bool fromValueMap(const cocos2d::ValueMap& map);

private:
    UndoActionType _actionType;         // 操作类型
    int _cardId;                         // 被操作的卡牌ID
    int _replacedCardId;                 // 被替换的卡牌ID（仅用于替换操作）
    cocos2d::Vec2 _cardPosition;        // 卡牌位置（用于回退动画）
    cocos2d::Vec2 _replacedCardPosition; // 被替换卡牌位置（仅用于替换操作）
    CardModel* _cardModel;               // 卡牌数据（需要深拷贝）
    CardModel* _replacedCardModel;       // 被替换的卡牌数据（仅用于替换操作，需要深拷贝）
};

#endif // __UNDO_MODEL_H__

