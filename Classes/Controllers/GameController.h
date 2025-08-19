#ifndef GameController_h
#define GameController_h
#include "../Models/GameModel.h"
#include "../Views/GameView.h"
#include <memory>
#include <stack>

// 操作记录（用于撤销）
struct GameAction {
    enum ActionType {
        ACTION_MOVE_CARD,    // 移动卡牌
        ACTION_FLIP_CARD,    // 翻牌
        ACTION_REMOVE_CARDS  // 移除卡牌
    };
    
    ActionType type;       // 操作类型
    int cardId1;           // 卡牌1 ID
    int cardId2;           // 卡牌2 ID（用于匹配移除）
    float fromX, fromY;    // 起始位置
    float toX, toY;        // 目标位置
    bool wasInPlayfield;   // 是否在游戏区
};

class GameController {
private:
    std::unique_ptr<GameModel> _gameModel;  // 游戏模型
    GameView* _gameView;                    // 游戏视图
    std::stack<GameAction> _actionHistory;  // 操作历史（用于撤销）
    int _nextCardId;                        // 下一个卡牌ID
    
public:
    // 构造函数
    GameController(int levelId);
    
    // 析构函数
    ~GameController() = default;
    
    // 获取游戏视图
    GameView* getGameView() const;
    
    // 开始游戏
    void startGame();
    
    // 处理卡牌点击事件
    void handleCardClick(int cardId);
    
    // 撤销上一步操作
    void undoLastAction();
    
    // 重新开始游戏
    void restartGame();
    
private:
    // 初始化游戏模型
    void initializeGameModel(int levelId);
    
    // 初始化游戏视图
    void initializeGameView();
    
    // 检查卡牌是否可以移动
    bool canMoveCard(int cardId) const;
    
    // 移动卡牌
    void moveCard(int cardId, float x, float y, bool toPlayfield);
    
    // 检查是否有匹配的卡牌
    bool checkForMatches(int cardId);
    
    // 移除匹配的卡牌
    void removeMatchingCards(int cardId1, int cardId2);
    
    // 记录操作（用于撤销）
    void recordAction(const GameAction& action);
    
    // 检查游戏状态
    void checkGameState();
};

#endif
