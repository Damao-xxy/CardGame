#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"
#include "Controllers/GameController.h"

class GameScene : public cocos2d::Scene {
private:
    std::unique_ptr<GameController> _gameController;  // 游戏控制器
    
public:
    // 创建场景（带levelId参数）
    static GameScene* createScene(int levelId);
    
    // 初始化（带levelId参数）
    bool init(int levelId);  // 明确标记为重写父类方法
    
    // 析构函数（释放资源）
    ~GameScene() override;
};

#endif
    
