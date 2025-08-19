#ifndef LevelSelectScene_h
#define LevelSelectScene_h

#include "cocos2d.h"
#include <functional>

class LevelSelectScene : public cocos2d::Scene {
private:
    std::function<void(int)> _levelSelectedCallback;  // 关卡选择回调
    
public:
    // 创建场景
    static LevelSelectScene* createScene(const std::function<void(int)>& callback);
    
    // 初始化
    bool init() override;
    
    // 设置关卡选择回调
    void setLevelSelectedCallback(const std::function<void(int)>& callback);
    
    // 实现CREATE_FUNC宏
    CREATE_FUNC(LevelSelectScene);
    
private:
    // 关卡按钮点击事件
    void onLevelButtonClicked(cocos2d::Ref* sender);
    
    // 初始化UI
    void initUI();
};

#endif
