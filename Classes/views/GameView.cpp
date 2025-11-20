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

#include "GameView.h"
#include "../models/GameModel.h"
#include "GameResultView.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"

USING_NS_CC;

GameView* GameView::create(const GameModel* gameModel)
{
    GameView* view = new GameView();
    if (view && view->init(gameModel))
    {
        view->autorelease();
        return view;
    }
    delete view;
    return nullptr;
}

bool GameView::init(const GameModel* gameModel)
{
    if (!Scene::init() || gameModel == nullptr)
    {
        return false;
    }
    
    _gameModel = gameModel;
    _playFieldView = nullptr;
    _stackView = nullptr;
    _bottomCardView = nullptr;
    _undoButton = nullptr;
    _gameResultView = nullptr;
    
    // 创建背景（分上下两个区域）
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    // 上方区域：浅棕色背景（主牌堆区域，约2/3屏幕）
    float topAreaHeight = visibleSize.height * 0.67f;  // 约2/3
    auto topBackground = LayerColor::create(Color4B(210, 180, 140, 255), visibleSize.width, topAreaHeight);
    topBackground->setPosition(Vec2(0, visibleSize.height - topAreaHeight));
    this->addChild(topBackground, 0);
    
    // 下方区域：紫色背景（备用牌堆和底牌区域，约1/3屏幕）
    float bottomAreaHeight = visibleSize.height * 0.33f;  // 约1/3
    auto bottomBackground = LayerColor::create(Color4B(128, 0, 128, 255), visibleSize.width, bottomAreaHeight);
    bottomBackground->setPosition(Vec2(0, 0));
    this->addChild(bottomBackground, 0);
    
    // 创建主牌堆视图（上方区域）
    _playFieldView = PlayFieldView::create(gameModel);
    if (_playFieldView != nullptr)
    {
        _playFieldView->setContentSize(Size(visibleSize.width, topAreaHeight));
        _playFieldView->setPosition(Vec2(0, visibleSize.height - topAreaHeight));
        this->addChild(_playFieldView, 1);
        // 设置完视图尺寸后，调用updateView确保位置正确
        _playFieldView->updateView(gameModel);
    }
    
    // 创建备用牌堆视图（下方左边，可以显示多张牌，部分重叠）
    _stackView = StackView::create(gameModel);
    if (_stackView != nullptr)
    {
        // 备用牌堆占下方区域的左半部分
        float stackWidth = visibleSize.width * 0.5f;
        _stackView->setContentSize(Size(stackWidth, bottomAreaHeight));
        _stackView->setPosition(Vec2(100.0f, 0));  // 左下角
        this->addChild(_stackView, 1);
        // 设置完视图尺寸后，调用updateView确保位置正确
        _stackView->updateView(gameModel);
    }
    
    // 创建底牌视图（下方右边，只显示一张牌）
    _bottomCardView = BottomCardView::create(gameModel);
    if (_bottomCardView != nullptr)
    {
        // 底牌占下方区域的右半部分
        float bottomWidth = visibleSize.width * 0.5f;
        _bottomCardView->setContentSize(Size(bottomWidth, bottomAreaHeight));
        _bottomCardView->setPosition(Vec2(visibleSize.width * 0.5f, 0));  // 右下角
        this->addChild(_bottomCardView, 1);
        // 设置完视图尺寸后，调用updateView确保位置正确
        _bottomCardView->updateView(gameModel);
    }
    
    initUI();
    
    return true;
}

void GameView::updateView(const GameModel* gameModel)
{
    if (gameModel == nullptr)
    {
        return;
    }
    
    _gameModel = gameModel;
    
    if (_playFieldView != nullptr)
    {
        _playFieldView->updateView(gameModel);
    }
    
    if (_stackView != nullptr)
    {
        _stackView->updateView(gameModel);
    }
    
    if (_bottomCardView != nullptr)
    {
        _bottomCardView->updateView(gameModel);
    }
}

void GameView::setUndoButtonCallback(const std::function<void()>& callback)
{
    _undoButtonCallback = callback;
}

void GameView::initUI()
{
    // 创建回退按钮（右下角，白色文字，显示"回退"两个字）
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    // 使用系统字体创建标签，确保中文显示正确
    auto undoLabel = Label::createWithSystemFont("回退", "", 48);
    if (undoLabel == nullptr)
    {
        // 如果系统字体失败，尝试使用TTF字体
        undoLabel = Label::createWithTTF("回退", "fonts/Marker Felt.ttf", 48);
    }
    
    if (undoLabel != nullptr)
    {
        undoLabel->setColor(Color3B::WHITE);  // 白色文字
        _undoButton = MenuItemLabel::create(undoLabel, 
                                            CC_CALLBACK_1(GameView::onUndoButtonClicked, this));
        // 右下角位置
        _undoButton->setPosition(Vec2(visibleSize.width - 100, 100));
        
        auto menu = Menu::create(_undoButton, nullptr);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 10);
    }
}

void GameView::onUndoButtonClicked(Ref* sender)
{
    if (_undoButtonCallback != nullptr)
    {
        _undoButtonCallback();
    }
}

void GameView::showGameResult(GameResultType resultType)
{
    // 如果已经显示了结果弹窗，不再重复显示
    if (_gameResultView != nullptr)
    {
        return;
    }
    
    // 创建游戏结果弹窗
    _gameResultView = GameResultView::create(resultType);
    if (_gameResultView != nullptr)
    {
        // 复制回调函数，避免悬空指针
        std::function<void()> restartCallback = _restartCallback;
        std::function<void()> exitCallback = _exitCallback;
        
        // 设置回调（完全不依赖this，只使用复制的回调）
        // 注意：弹窗的移除由 GameController::restartGame() 处理，这里只执行回调
        _gameResultView->setRestartCallback([restartCallback]() {
            // 直接执行回调，不访问任何对象成员
            if (restartCallback != nullptr)
            {
                restartCallback();
            }
        });
        
        _gameResultView->setExitCallback([exitCallback]() {
            // 直接执行回调，不访问任何对象成员
            if (exitCallback != nullptr)
            {
                exitCallback();
            }
        });
        
        this->addChild(_gameResultView, 100);  // 最高层级
    }
}

void GameView::removeGameResultView()
{
    if (_gameResultView != nullptr)
    {
        _gameResultView->removeFromParent();
        _gameResultView = nullptr;
    }
}

