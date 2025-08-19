#ifndef GameModel_h
#define GameModel_h

#include "CardModel.h"
#include "../Constants.h"
#include <vector>
#include <memory>

class GameModel {
private:
    int _levelId;                  // 关卡ID
    GameState _gameState;          // 游戏状态
    std::vector<std::unique_ptr<CardModel>> _playfieldCards;  // 游戏区卡牌
    std::vector<std::unique_ptr<CardModel>> _stackCards;      // 牌堆卡牌
    int _movesCount;               // 移动次数
    int _score;                    // 分数
    
public:
    // 构造函数
    GameModel(int levelId);
    
    // 析构函数
    ~GameModel() = default;
    
    // 获取关卡ID
    int getLevelId() const;
    
    // 获取游戏状态
    GameState getGameState() const;
    
    // 设置游戏状态
    void setGameState(GameState state);
    
    // 添加游戏区卡牌
    void addPlayfieldCard(std::unique_ptr<CardModel> card);
    
    // 添加牌堆卡牌
    void addStackCard(std::unique_ptr<CardModel> card);
    
    // 获取游戏区所有卡牌
    const std::vector<std::unique_ptr<CardModel>>& getPlayfieldCards() const;
    
    // 获取牌堆所有卡牌
    const std::vector<std::unique_ptr<CardModel>>& getStackCards() const;
    
    // 根据ID查找卡牌
    CardModel* findCardById(int cardId);
    
    // 将卡牌从游戏区移至牌堆
    bool moveCardFromPlayfieldToStack(int cardId);
    
    // 将卡牌从牌堆移至游戏区
    bool moveCardFromStackToPlayfield(int cardId, float x, float y);
    
    // 获取移动次数
    int getMovesCount() const;
    
    // 增加移动次数
    void incrementMovesCount();
    
    // 获取分数
    int getScore() const;
    
    // 增加分数
    void addScore(int points);
    
    // 检查游戏是否结束
    bool isGameOver() const;
    
    // 检查游戏是否胜利
    bool isGameWon() const;
};

#endif
