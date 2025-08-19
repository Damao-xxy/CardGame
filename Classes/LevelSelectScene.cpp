//#include "LevelSelectScene.h"
////#include "cocos2d/ui/CocosGUI.h"
//#include "../cocos2d/cocos/ui/CocosGUI.h"
//
//using namespace cocos2d;
//using namespace cocos2d::ui;
//
//LevelSelectScene* LevelSelectScene::createScene(const std::function<void(int)>& callback) {
//    LevelSelectScene* scene = new (std::nothrow) LevelSelectScene();
//    if (scene && scene->init()) {
//        scene->setLevelSelectedCallback(callback);
//        scene->autorelease();
//        return scene;
//    }
//    CC_SAFE_DELETE(scene);
//    return nullptr;
//}
//
//bool LevelSelectScene::init() {
//    if (!Scene::init()) {
//        return false;
//    }
//    
//    // 初始化UI
//    initUI();
//    
//    return true;
//}
//
//void LevelSelectScene::setLevelSelectedCallback(const std::function<void(int)>& callback) {
//    _levelSelectedCallback = callback;
//}
//
//void LevelSelectScene::onLevelButtonClicked(Ref* sender) {
//    Button* button = dynamic_cast<Button*>(sender);
//    if (button && _levelSelectedCallback) {
//        int levelId = button->getTag();
//        _levelSelectedCallback(levelId);
//    }
//}
//
//void LevelSelectScene::initUI() {
//    Size visibleSize = Director::getInstance()->getVisibleSize();
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//    
//    // 创建背景
//    auto background = Sprite::create("background.png");
//    background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
//    addChild(background);
//    
//    // 创建标题
//    auto title = Label::createWithTTF("Select Level", "fonts/Marker Felt.ttf", 48);
//    title->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height - 50 + origin.y));
//    title->setColor(Color3B::YELLOW);
//    addChild(title);
//    
//    // 创建关卡按钮
//    int levelCount = 5;
//    int buttonsPerRow = 3;
//    float startX = 150;
//    float startY = visibleSize.height - 150;
//    float offsetX = 200;
//    float offsetY = 150;
//    
//    for (int i = 0; i < levelCount; i++) {
//        int row = i / buttonsPerRow;
//        int col = i % buttonsPerRow;
//        
//        auto button = Button::create("CloseNormal.png", "CloseSelected.png");
//        button->setPosition(Vec2(startX + col * offsetX, startY - row * offsetY));
//        button->setTag(i + 1);  // 关卡ID从1开始
//        button->addClickEventListener(CC_CALLBACK_1(LevelSelectScene::onLevelButtonClicked, this));
//        
//        // 添加关卡编号
//        auto label = Label::createWithTTF(StringUtils::format("%d", i + 1), "fonts/Marker Felt.ttf", 36);
//        label->setPosition(Vec2(button->getContentSize().width / 2, button->getContentSize().height / 2));
//        label->setColor(Color3B::BLACK);
//        button->addChild(label);
//        
//        addChild(button);
//    }
//}
#include "LevelSelectScene.h"
#include "../cocos2d/cocos/ui/CocosGUI.h"

using namespace cocos2d;
using namespace cocos2d::ui;

LevelSelectScene* LevelSelectScene::createScene(const std::function<void(int)>& callback) {
    LevelSelectScene* scene = new (std::nothrow) LevelSelectScene();
    if (scene && scene->init()) {
        scene->setLevelSelectedCallback(callback);
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

bool LevelSelectScene::init() {
    if (!Scene::init()) {
        return false;
    }
    
    // 初始化UI
    initUI();
    
    return true;
}

void LevelSelectScene::setLevelSelectedCallback(const std::function<void(int)>& callback) {
    _levelSelectedCallback = callback;
}

void LevelSelectScene::onLevelButtonClicked(Ref* sender) {
    Button* button = dynamic_cast<Button*>(sender);
    if (button && _levelSelectedCallback) {
        int levelId = button->getTag();
        _levelSelectedCallback(levelId);
    }
}

void LevelSelectScene::initUI() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 创建背景（保持不变，已居中）
    auto background = Sprite::create("background.png");
    if (background) {
        float scaleX = visibleSize.width / background->getContentSize().width;
        float scaleY = visibleSize.height / background->getContentSize().height;
        float maxScale = MAX(scaleX, scaleY);
        background->setScale(maxScale);
        background->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        addChild(background, -10);
    }
    
    // 创建标题（已水平居中，保持不变）
    auto title = Label::createWithTTF("Select Level", "fonts/Marker Felt.ttf", 48);
    title->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height - 50 + origin.y));
    title->setColor(Color3B::YELLOW);
    addChild(title);
    
    // 创建关卡按钮 - 整体居中逻辑
    int levelCount = 5;
    int buttonsPerRow = 5;  // 每行2个按钮
    float buttonScale = 2.0f;  // 按钮缩放倍数
    float buttonWidth = 0;    // 按钮实际宽度（后续计算）
    float buttonHeight = 0;   // 按钮实际高度（后续计算）
    
    // 1. 先创建一个临时按钮获取尺寸（用于计算布局）
    auto tempButton = Button::create("CloseNormal.png");
    if (tempButton) {
        buttonWidth = tempButton->getContentSize().width * buttonScale;  // 实际宽度 = 原图宽度 * 缩放
        buttonHeight = tempButton->getContentSize().height * buttonScale; // 实际高度 = 原图高度 * 缩放
    } else {
        // 若图片加载失败，用默认尺寸
        buttonWidth = 60 * buttonScale;
        buttonHeight = 60 * buttonScale;
    }
    
    // 2. 计算按钮间距（根据按钮尺寸动态调整）
    float spacingX = buttonWidth * 0.5f;  // 水平间距 = 按钮宽度的一半
    float spacingY = buttonHeight * 0.5f; // 垂直间距 = 按钮高度的一半
    
    // 3. 计算按钮区域总宽度（用于居中）
    // 总宽度 = 按钮宽度*数量 + 间距*(数量-1)
    float totalWidth = buttonsPerRow * buttonWidth + (buttonsPerRow - 1) * spacingX;
    
    // 4. 计算起始X坐标（让按钮区域水平居中）
    float startX = (visibleSize.width - totalWidth) / 2 + origin.x + buttonWidth / 2;
    // 计算起始Y坐标（垂直方向居中偏上，与标题保持距离）
    float startY = visibleSize.height * 0.6f;  // 屏幕高度的60%位置
    
    // 5. 循环创建按钮
    for (int i = 0; i < levelCount; i++) {
        int row = i / buttonsPerRow;  // 当前行
        int col = i % buttonsPerRow;  // 当前列
        
        auto button = Button::create("CloseNormal.png", "CloseSelected.png");
        button->setScale(buttonScale);  // 放大按钮
        button->setTag(i + 1);          // 关卡ID
        button->addClickEventListener(CC_CALLBACK_1(LevelSelectScene::onLevelButtonClicked, this));
        
        // 计算按钮位置（基于居中的起始坐标）
        float posX = startX + col * (buttonWidth + spacingX);
        float posY = startY - row * (buttonHeight + spacingY);
        button->setPosition(Vec2(posX, posY));
        
        // 添加关卡文字
        auto label = Label::createWithTTF(StringUtils::format("%d", i + 1), "fonts/Marker Felt.ttf", 48);
        label->setPosition(Vec2(button->getContentSize().width / 2, button->getContentSize().height / 2));
        label->setColor(Color3B::BLACK);
        button->addChild(label);
        
        addChild(button);
    }
}
