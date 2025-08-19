#include "GameScene.h"

using namespace cocos2d;

GameScene* GameScene::createScene(int levelId) {
    GameScene* scene = new (std::nothrow) GameScene();
    if (scene && scene->init(levelId)) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

bool GameScene::init(int levelId) {
    if (!Scene::init()) {
        return false;
    }
    
    // 创建游戏控制器
    _gameController = std::make_unique<GameController>(levelId);
    
    // 添加游戏视图到场景
    if (_gameController->getGameView()) {
        addChild(_gameController->getGameView());
        // 开始游戏
        _gameController->startGame();
    }
    
    return true;
}
// 必须实现析构函数
GameScene::~GameScene() {
    // 即使为空，也需要显式实现
}
