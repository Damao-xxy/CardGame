#include "../Models/GameModel.h"
#include "../Models/CardModel.h"
#include "GameController.h"
#include "cocos2d.h"
#include <random>
#include <chrono>

using namespace cocos2d;

GameController::GameController(int levelId)
    : _gameModel(std::make_unique<GameModel>(levelId)), 
      _gameView(nullptr), _nextCardId(1) {
    // 初始化游戏模型
    initializeGameModel(levelId);
    // 初始化游戏视图
    initializeGameView();
}

GameView* GameController::getGameView() const {
    return _gameView;
}

void GameController::startGame() {
    // 设置游戏状态为进行中
    _gameModel->setGameState(GAME_STATE_PLAYING);
    // 播放入场动画
    _gameView->playEnterAnimation();
}

//void GameController::handleCardClick(int cardId) {
//    // 如果游戏不在进行中，不处理点击
//    if (_gameModel->getGameState() != GAME_STATE_PLAYING) {
//        return;
//    }
//    
//    CardModel* card = _gameModel->findCardById(cardId);
//    if (!card) {
//        return;
//    }
//    
//    // 如果卡牌是背面朝上，翻转它
//    if (card->getState() == CARD_STATE_FACE_DOWN) {
//        card->setState(CARD_STATE_FACE_UP);
//        _gameView->updateWithModel(_gameModel.get());
//        
//        // 记录翻牌操作
//        GameAction action;
//        action.type = GameAction::ACTION_FLIP_CARD;
//        action.cardId1 = cardId;
//        action.cardId2 = -1;
//        recordAction(action);
//        
//        // 检查是否有匹配
//        if (checkForMatches(cardId)) {
//            checkGameState();
//        }
//    }
//    // 如果卡牌可以移动，移动它
//    else if (canMoveCard(cardId)) {
//        // 这里可以实现拖拽移动逻辑，简单处理为移动到特定位置
//        float targetX = 500;
//        float targetY = 300;
//        moveCard(cardId, targetX, targetY, true);
//        checkGameState();
//    }
//}

void GameController::handleCardClick(int cardId) {
    // 如果游戏不在进行中，不处理点击
    if (_gameModel->getGameState() != GAME_STATE_PLAYING) {
        return;
    }
    
    CardModel* card = _gameModel->findCardById(cardId);
    if (!card) {
        return;
    }
    
    // 记录原始状态和位置（统一用于所有情况的回退）
//    CardState originalState = card->getState();
    float originalX = card->getPositionX();
    float originalY = card->getPositionY();
    
    // 情况1：牌在牌堆区且背面朝上
    if (card->getZone() == CARD_ZONE_STACK && card->getState() == CARD_STATE_FACE_DOWN) {
        // 翻转卡牌（正面朝上）
        card->setState(CARD_STATE_FACE_UP);
        
        // 向右移动两个卡牌宽度
        float moveDistance = kCardWidth;
        float newX = originalX + 2 * moveDistance;
        float newY = originalY; // Y轴位置不变
        card->setPosition(newX, newY);
        
        // 记录翻牌+移动操作（统一格式）
        GameAction action;
        action.type = GameAction::ACTION_FLIP_CARD;
        action.cardId1 = cardId;
        action.fromX = originalX;
        action.fromY = originalY;
        action.toX = newX;
        action.toY = newY;
        recordAction(action);
        
        // 更新视图并播放移动动画
        _gameView->updateWithModel(_gameModel.get());
        //        _gameView->getCardView(cardId)->runMoveAnimation(Vec2(newX, newY), 0.3f);
    }
    // 情况2：牌在游戏区且背面朝上
    else if (card->getZone() == CARD_ZONE_PLAYFIELD && card->getState() == CARD_STATE_FACE_DOWN) {
        card->setState(CARD_STATE_FACE_UP);
        _gameView->updateWithModel(_gameModel.get());

        // 记录翻牌操作
        GameAction action;
        action.type = GameAction::ACTION_FLIP_CARD;
        action.cardId1 = cardId;
        action.cardId2 = -1;
        recordAction(action);
        
        // 检查是否有匹配
        if (checkForMatches(cardId)) {
            checkGameState();
        }
        
    }
}
void GameController::undoLastAction() {
    if (_actionHistory.empty() || _gameModel->getGameState() != GAME_STATE_PLAYING) {
        return;
    }
    
    GameAction action = _actionHistory.top();
    _actionHistory.pop();
    
    switch (action.type) {
        case GameAction::ACTION_MOVE_CARD: {
            // 撤销移动操作
            CardModel* card = _gameModel->findCardById(action.cardId1);
            if (card) {
                if (action.wasInPlayfield) {
                    _gameModel->moveCardFromStackToPlayfield(action.cardId1, action.fromX, action.fromY);
                    _gameView->playMoveAnimation(action.cardId1, action.fromX, action.fromY, true);
                } else {
                    _gameModel->moveCardFromPlayfieldToStack(action.cardId1);
                    _gameView->playMoveAnimation(action.cardId1, action.fromX, action.fromY, false);
                }
                _gameModel->incrementMovesCount(); // 撤销也算一次移动
            }
            break;
        }
        case GameAction::ACTION_FLIP_CARD: {
            // 撤销翻牌操作
            CardModel* card = _gameModel->findCardById(action.cardId1);
            if (card) {
                card->setState(CARD_STATE_FACE_DOWN);
                _gameView->updateWithModel(_gameModel.get());
            }
            break;
        }
        case GameAction::ACTION_REMOVE_CARDS: {
            // 撤销移除操作（这里简化处理，实际游戏中可能需要更复杂的逻辑）
            break;
        }
    }
    
    _gameView->updateWithModel(_gameModel.get());
}

void GameController::restartGame() {
    int levelId = _gameModel->getLevelId();
    // 清空操作历史
    while (!_actionHistory.empty()) {
        _actionHistory.pop();
    }
    // 重新初始化游戏模型和视图
    _gameModel = std::make_unique<GameModel>(levelId);
    initializeGameModel(levelId);
    initializeGameView();
    startGame();
}

void GameController::initializeGameModel(int levelId) {
    // 根据关卡ID初始化不同的卡牌配置
    // 这里简单处理，创建一副牌并分配到游戏区和牌堆
    
    // 创建卡牌
    std::vector<std::unique_ptr<CardModel>> cards;
    int _nextCardId = 1; // 假设卡片ID从1开始
    
    // 创建8张牌（4对匹配的牌）
       for (int i = 0; i < 2; i++) {
           // 枚举循环需通过int类型中转
           for (int s = static_cast<int>(SUIT_CLUBS); s < static_cast<int>(SUIT_COUNT); s++) {
               for (int r = static_cast<int>(RANK_A); r < static_cast<int>(RANK_5); r++) {
                   for(int c = 0; c<2; c++){
                       // 转换回枚举类型
                       CardSuit suit = static_cast<CardSuit>(s);
                       CardRank rank = static_cast<CardRank>(r);
                       std::string color = (c==0) ? "red":"black";
                       // 创建unique_ptr并添加到容器
                       auto card = std::make_unique<CardModel>(_nextCardId++, suit, rank, color);
                       cards.push_back(std::move(card));
                   }
               }
           }
       }
    
    // 随机打乱卡牌
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
    
    // 分配卡牌到游戏区和牌堆
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    // 前8张放到游戏区
    float startX = 100;
    float startY = visibleSize.height - 200;
    float offsetX = kCardWidth + 20;
    float offsetY = kCardHeight + 20;
    int cardsPerRow = 4;
    
    for (int i = 0; i < 8 && i < cards.size(); i++) {
        int row = i / cardsPerRow;
        int col = i % cardsPerRow;
        float x = startX + col * offsetX;
        float y = startY - row * offsetY;
        
        cards[i]->setPosition(x, y);
        cards[i]->setState(CARD_STATE_FACE_DOWN);
        cards[i]->setZone(CARD_ZONE_PLAYFIELD);
        cards[i]->setMovable(true);
        
        _gameModel->addPlayfieldCard(std::move(cards[i]));
    }
    
    // 剩余的放到牌堆
    for (int i = 8; i < cards.size(); i++) {
        cards[i]->setPosition(100, 100);
        cards[i]->setState(CARD_STATE_FACE_DOWN);
        cards[i]->setZone(CARD_ZONE_STACK);
        cards[i]->setMovable(true);
        
        _gameModel->addStackCard(std::move(cards[i]));
    }
}

void GameController::initializeGameView() {
    // 创建游戏视图
    _gameView = GameView::create();
    
    // 设置回调
    _gameView->setCardClickCallback([this](int cardId) {
        handleCardClick(cardId);
    });
    
    _gameView->setUndoCallback([this]() {
        undoLastAction();
    });
    
    _gameView->setRestartCallback([this]() {
        restartGame();
    });
    
    // 添加游戏区卡牌视图
    for (const auto& cardModel : _gameModel->getPlayfieldCards()) {
        auto cardView = CardView::create(
            cardModel->getCardId(),
            cardModel->getSuit(),
            cardModel->getRank(),
            cardModel->getColor()
        );
        _gameView->addCardView(cardView, true);
    }
    
    // 添加牌堆卡牌视图
    for (const auto& cardModel : _gameModel->getStackCards()) {
        auto cardView = CardView::create(
            cardModel->getCardId(),
            cardModel->getSuit(),
            cardModel->getRank(),
            cardModel->getColor()
        );
        _gameView->addCardView(cardView, false);
    }
    
    // 更新视图
    _gameView->updateWithModel(_gameModel.get());
}

bool GameController::canMoveCard(int cardId) const {
    const CardModel* card = _gameModel->findCardById(cardId);
    if (!card) {
        return false;
    }
    
    // 卡牌可以移动的条件：正面朝上且标记为可移动
    return card->isMovable() && card->getState() == CARD_STATE_FACE_UP;
}

void GameController::moveCard(int cardId, float x, float y, bool toPlayfield) {
    CardModel* card = _gameModel->findCardById(cardId);
    if (!card) {
        return;
    }
    
    // 记录原始位置和状态
    float fromX = card->getPositionX();
    float fromY = card->getPositionY();
    bool wasInPlayfield = false;
    
    // 查找卡牌当前所在区域
    for (const auto& playfieldCard : _gameModel->getPlayfieldCards()) {
        if (playfieldCard->getCardId() == cardId) {
            wasInPlayfield = true;
            break;
        }
    }
    
    // 执行移动
    if (toPlayfield) {
        _gameModel->moveCardFromStackToPlayfield(cardId, x, y);
    } else {
        _gameModel->moveCardFromPlayfieldToStack(cardId);
    }
    
    // 更新视图
    _gameView->playMoveAnimation(cardId, x, y, toPlayfield);
    _gameView->updateWithModel(_gameModel.get());
    
    // 记录操作
    GameAction action;
    action.type = GameAction::ACTION_MOVE_CARD;
    action.cardId1 = cardId;
    action.cardId2 = -1;
    action.fromX = fromX;
    action.fromY = fromY;
    action.toX = x;
    action.toY = y;
    action.wasInPlayfield = wasInPlayfield;
    recordAction(action);
}

bool GameController::checkForMatches(int cardId) {
    CardModel* card = _gameModel->findCardById(cardId);
    if (!card) {
        return false;
    }
    
    // 查找游戏区中与当前卡牌匹配的其他正面朝上的卡牌
    for (const auto& otherCard : _gameModel->getStackCards()) {
        if (otherCard->getCardId() != cardId &&
            otherCard->getState() == CARD_STATE_FACE_UP &&
            card->matches(otherCard.get())) {
            
            // 找到匹配的卡牌
            removeMatchingCards(cardId, otherCard->getCardId());
            return true;
        }
    }
    
    return false;
}

void GameController::removeMatchingCards(int cardId1, int cardId2) {
    // 播放匹配动画
    _gameView->playMatchAnimation(cardId1, cardId2);
    
    // 更新模型（标记为已移除）
    CardModel* card1 = _gameModel->findCardById(cardId1);
    CardModel* card2 = _gameModel->findCardById(cardId2);
    
    if (card1 && card2) {
        card1->setState(CARD_STATE_REMOVED);
        card2->setState(CARD_STATE_REMOVED);
        
        // 增加分数
        _gameModel->addScore(100);
        
        // 记录操作
        GameAction action;
        action.type = GameAction::ACTION_REMOVE_CARDS;
        action.cardId1 = cardId1;
        action.cardId2 = cardId2;
        recordAction(action);
    }
}

void GameController::recordAction(const GameAction& action) {
    _actionHistory.push(action);
}

void GameController::checkGameState() {
    if (_gameModel->isGameWon()) {
        _gameModel->setGameState(GAME_STATE_WIN);
        _gameView->showGameOverDialog(true);
    } else if (_gameModel->isGameOver()) {
        _gameModel->setGameState(GAME_STATE_LOSE);
        _gameView->showGameOverDialog(false);
    }
}
