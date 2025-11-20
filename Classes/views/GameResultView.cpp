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

#include "GameResultView.h"
#include "2d/CCLayer.h"
#include "2d/CCLabel.h"
#include "base/CCDirector.h"

USING_NS_CC;

GameResultView* GameResultView::create(GameResultType resultType)
{
    GameResultView* view = new GameResultView();
    if (view && view->init(resultType))
    {
        view->autorelease();
        return view;
    }
    delete view;
    return nullptr;
}

bool GameResultView::init(GameResultType resultType)
{
    if (!Layer::init())
    {
        return false;
    }
    
    _resultType = resultType;
    _restartCallback = nullptr;
    _exitCallback = nullptr;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    // 创建半透明黑色背景
    auto background = LayerColor::create(Color4B(0, 0, 0, 180), visibleSize.width, visibleSize.height);
    background->setPosition(Vec2::ZERO);
    this->addChild(background, 0);
    
    // 创建弹窗背景（白色，圆角效果）
    float popupWidth = 600.0f;
    float popupHeight = 400.0f;
    auto popupBg = LayerColor::create(Color4B(255, 255, 255, 255), popupWidth, popupHeight);
    popupBg->setPosition(Vec2((visibleSize.width - popupWidth) / 2, (visibleSize.height - popupHeight) / 2));
    this->addChild(popupBg, 1);
    
    // 创建标题文本
    std::string titleText;
    if (_resultType == GameResultType::VICTORY)
    {
        titleText = "\xe9\x97\xaf\xe5\x85\xb3\xe6\x88\x90\xe5\x8a\x9f";  // "闯关成功" UTF-8编码
    }
    else
    {
        titleText = "\xe9\x97\xaf\xe5\x85\xb3\xe5\xa4\xb1\xe8\xb4\xa5";  // "闯关失败" UTF-8编码
    }
    
    auto titleLabel = Label::createWithSystemFont(titleText, "", 72);
    if (titleLabel == nullptr)
    {
        titleLabel = Label::createWithTTF(titleText, "fonts/Marker Felt.ttf", 72);
    }
    if (titleLabel != nullptr)
    {
        titleLabel->setColor(_resultType == GameResultType::VICTORY ? Color3B(0, 200, 0) : Color3B(200, 0, 0));
        titleLabel->setPosition(Vec2(popupWidth / 2, popupHeight * 0.7f));
        popupBg->addChild(titleLabel, 2);
    }
    
    // 创建"再来一局"按钮
    std::string restartText("\xe5\x86\x8d\xe6\x9d\xa5\xe4\xb8\x80\xe5\xb1\x80");  // "再来一局" UTF-8编码
    auto restartLabel = Label::createWithSystemFont(restartText, "", 48);
    if (restartLabel == nullptr)
    {
        restartLabel = Label::createWithTTF(restartText, "fonts/Marker Felt.ttf", 48);
    }
    if (restartLabel != nullptr)
    {
        restartLabel->setColor(Color3B::WHITE);
        auto restartButton = MenuItemLabel::create(restartLabel, 
                                                   CC_CALLBACK_1(GameResultView::onRestartButtonClicked, this));
        restartButton->setPosition(Vec2(popupWidth / 2 - 120, popupHeight * 0.3f));
        
        // 创建按钮背景
        auto restartBg = LayerColor::create(Color4B(0, 150, 0, 255), 200, 60);
        restartBg->setPosition(Vec2(popupWidth / 2 - 220, popupHeight * 0.3f - 30));
        popupBg->addChild(restartBg, 1);
        
        // 创建"退出"按钮
        std::string exitText("\xe9\x80\x80\xe5\x87\xba");  // "退出" UTF-8编码
        auto exitLabel = Label::createWithSystemFont(exitText, "", 48);
        if (exitLabel == nullptr)
        {
            exitLabel = Label::createWithTTF(exitText, "fonts/Marker Felt.ttf", 48);
        }
        if (exitLabel != nullptr)
        {
            exitLabel->setColor(Color3B::WHITE);
            auto exitButton = MenuItemLabel::create(exitLabel, 
                                                     CC_CALLBACK_1(GameResultView::onExitButtonClicked, this));
            exitButton->setPosition(Vec2(popupWidth / 2 + 120, popupHeight * 0.3f));
            
            // 创建按钮背景
            auto exitBg = LayerColor::create(Color4B(150, 0, 0, 255), 200, 60);
            exitBg->setPosition(Vec2(popupWidth / 2 + 20, popupHeight * 0.3f - 30));
            popupBg->addChild(exitBg, 1);
            
            auto menu = Menu::create(restartButton, exitButton, nullptr);
            menu->setPosition(Vec2::ZERO);
            popupBg->addChild(menu, 3);
        }
    }
    
    return true;
}

void GameResultView::onRestartButtonClicked(Ref* sender)
{
    if (_restartCallback != nullptr)
    {
        _restartCallback();
    }
}

void GameResultView::onExitButtonClicked(Ref* sender)
{
    if (_exitCallback != nullptr)
    {
        _exitCallback();
    }
}

