//#include "AppDelegate.h"
//#include "cocos2d.h"
//#include "LevelSelectScene.h"
//#include "GameScene.h"
//
//using namespace cocos2d;
//
//AppDelegate::AppDelegate() {
//}
//
//AppDelegate::~AppDelegate() 
//{
//}
//
//void AppDelegate::initGLContextAttrs() {
//    // Set OpenGL context attributes: red, green, blue, alpha, depth, stencil
//    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
//    GLView::setGLContextAttrs(glContextAttrs);
//}
//
//bool AppDelegate::applicationDidFinishLaunching() {
//    // initialize director
//    auto director = Director::getInstance();
//    auto glView = director->getOpenGLView();
//    if(!glView) {
//        glView = GLViewImpl::create("Card Game");
//        director->setOpenGLView(glView);
//    }
//
//    // Set screen size
//    glView->setFrameSize(960, 640);
//    glView->setDesignResolutionSize(960, 640, ResolutionPolicy::SHOW_ALL);
//
//    // turn on display FPS
//    director->setDisplayStats(false);
//
//    // set FPS. the default value is 1.0/60 if you don't call this
//    director->setAnimationInterval(1.0f / 60);
//
////    // 显示关卡选择场景
////    auto scene = LevelSelectScene::createScene([](int levelId) {
////        // 关卡选择后切换到游戏场景
////        auto gameScene = GameScene::createScene(levelId);
////        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, gameScene));
////    });
//    
////    director->runWithScene(scene);
//    
//    auto gameScene = GameScene::createScene(1);
//    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, gameScene));
//    director->runWithScene(gameScene);
//
//    return true;
//}
//
//// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
//void AppDelegate::applicationDidEnterBackground() {
//    Director::getInstance()->stopAnimation();
//
//    // if you use SimpleAudioEngine, it must be paused
//    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
//}
//
//// this function will be called when the app is active again
//void AppDelegate::applicationWillEnterForeground() {
//    Director::getInstance()->startAnimation();
//
//    // if you use SimpleAudioEngine, it must resume here
//    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
//}
#include "AppDelegate.h"
#include "cocos2d.h"
#include "LevelSelectScene.h"
#include "GameScene.h"

using namespace cocos2d;

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate()
{
}

void AppDelegate::initGLContextAttrs() {
    // 设置 OpenGL 上下文属性（保持默认）
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // 初始化导演
    auto director = Director::getInstance();
    auto glView = director->getOpenGLView();
    if(!glView) {
        // 创建窗口时不固定尺寸，适应设备屏幕
        glView = GLViewImpl::create("Card Game");
        director->setOpenGLView(glView);
    }

    // --------------------------
    // 核心：设置分辨率适配策略
    // --------------------------
    // 设计分辨率（游戏逻辑尺寸，如 960x640）
    const float designWidth = 960;
    const float designHeight = 640;
    
    // 设置设计分辨率
    glView->setDesignResolutionSize(designWidth, designHeight, ResolutionPolicy::FIXED_WIDTH);
    // 解释：
    // - FIXED_WIDTH：保持宽度与设计分辨率一致，高度按设备比例缩放（适合横屏游戏）
    // - 若为竖屏游戏，可改用 FIXED_HEIGHT，保持高度一致，宽度缩放

    // 获取设备实际屏幕尺寸（用于调试）
    auto frameSize = glView->getFrameSize();
    CCLOG("设备屏幕尺寸：%fx%f", frameSize.width, frameSize.height);
    CCLOG("设计分辨率：%fx%f", designWidth, designHeight);

    // 关闭 FPS 显示（发布时保持关闭）
    director->setDisplayStats(false);

    // 设置帧率（保持默认）
    director->setAnimationInterval(1.0f / 60);

    // 启动场景（关卡选择场景）
    auto scene = LevelSelectScene::createScene([](int levelId) {
        // 切换到游戏场景时使用过渡动画
        auto gameScene = GameScene::createScene(levelId);
        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, gameScene));
    });
    director->runWithScene(scene);

    return true;
}

// 应用进入后台
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    // 暂停音频（如果使用）
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// 应用返回前台
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    // 恢复音频（如果使用）
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
