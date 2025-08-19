#ifndef GameView_h
#define GameView_h

#include "cocos2d.h"
#include "CardView.h"
#include "../Models/GameModel.h"
#include <vector>
#include <unordered_map>

class GameView : public cocos2d::Layer {
private:
    cocos2d::Layer* _playfieldLayer;       // 游戏区图层
    cocos2d::Layer* _stackLayer;           // 牌堆图层
    cocos2d::Label* _scoreLabel;           // 分数标签
    cocos2d::Label* _movesLabel;           // 移动次数标签
    cocos2d::MenuItem* _undoButton;        // 撤销按钮
    cocos2d::MenuItem* _restartButton;     // 重新开始按钮
    
    std::unordered_map<int, CardView*> _cardViews;  // 卡牌视图映射
    
    // 回调函数
    std::function<void(int)> _cardClickCallback;    // 卡牌点击回调
    std::function<void()> _undoCallback;            // 撤销回调
    std::function<void()> _restartCallback;         // 重新开始回调
    
public:
    // 创建游戏视图
    static GameView* create();
    
    // 构造函数
    GameView();
    
    // 析构函数
    ~GameView() override;
    
    // 初始化
    bool init() override;
    
    // 添加卡牌视图
    void addCardView(CardView* cardView, bool isInPlayfield);
    
    // 更新视图以匹配模型
    void updateWithModel(const GameModel* model);
    
    // 播放卡牌匹配动画
    void playMatchAnimation(int cardId1, int cardId2);
    
    // 播放卡牌移动动画
    void playMoveAnimation(int cardId, float x, float y, bool toPlayfield);
    
    // 播放入场动画
    void playEnterAnimation();
    
    // 设置卡牌点击回调
    void setCardClickCallback(const std::function<void(int)>& callback);
    
    // 设置撤销回调
    void setUndoCallback(const std::function<void()>& callback);
    
    // 设置重新开始回调
    void setRestartCallback(const std::function<void()>& callback);
    
    // 显示游戏结束对话框
    void showGameOverDialog(bool isWin);
    
private:
    // 初始化UI
    void initUI();
    
    // 初始化布局
    void initLayout();
    
    // 撤销按钮点击事件
    void onUndoClicked(cocos2d::Ref* sender);
    
    // 重新开始按钮点击事件
    void onRestartClicked(cocos2d::Ref* sender);
};

#endif
