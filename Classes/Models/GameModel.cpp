#include "GameModel.h"

GameModel::GameModel(int levelId)
    : _levelId(levelId), _gameState(GAME_STATE_INITIALIZING),
      _movesCount(0), _score(0) {}

int GameModel::getLevelId() const {
    return _levelId;
}

GameState GameModel::getGameState() const {
    return _gameState;
}

void GameModel::setGameState(GameState state) {
    _gameState = state;
}

void GameModel::addPlayfieldCard(std::unique_ptr<CardModel> card) {
    _playfieldCards.push_back(std::move(card));
}

void GameModel::addStackCard(std::unique_ptr<CardModel> card) {
    _stackCards.push_back(std::move(card));
}

const std::vector<std::unique_ptr<CardModel>>& GameModel::getPlayfieldCards() const {
    return _playfieldCards;
}

const std::vector<std::unique_ptr<CardModel>>& GameModel::getStackCards() const {
    return _stackCards;
}

CardModel* GameModel::findCardById(int cardId) {
    // 先在游戏区查找
    for (const auto& card : _playfieldCards) {
        if (card->getCardId() == cardId) {
            return card.get();
        }
    }
    
    // 再在牌堆查找
    for (const auto& card : _stackCards) {
        if (card->getCardId() == cardId) {
            return card.get();
        }
    }
    
    return nullptr;
}

bool GameModel::moveCardFromPlayfieldToStack(int cardId) {
    for (auto it = _playfieldCards.begin(); it != _playfieldCards.end(); ++it) {
        if ((*it)->getCardId() == cardId) {
            // 将卡牌从游戏区移至牌堆
            auto card = std::move(*it);
            _playfieldCards.erase(it);
            _stackCards.push_back(std::move(card));
            incrementMovesCount();
            return true;
        }
    }
    return false;
}

bool GameModel::moveCardFromStackToPlayfield(int cardId, float x, float y) {
    for (auto it = _stackCards.begin(); it != _stackCards.end(); ++it) {
        if ((*it)->getCardId() == cardId) {
            // 将卡牌从牌堆移至游戏区
            auto card = std::move(*it);
            _stackCards.erase(it);
            card->setPosition(x, y);
            _playfieldCards.push_back(std::move(card));
            incrementMovesCount();
            return true;
        }
    }
    return false;
}

int GameModel::getMovesCount() const {
    return _movesCount;
}

void GameModel::incrementMovesCount() {
    _movesCount++;
}

int GameModel::getScore() const {
    return _score;
}

void GameModel::addScore(int points) {
    _score += points;
}

bool GameModel::isGameOver() const {
    // 游戏结束条件：牌堆为空且没有可匹配的卡牌
    return _stackCards.empty() && isGameWon();
}

bool GameModel::isGameWon() const {
    // 胜利条件：游戏区没有卡牌
    return _playfieldCards.empty();
}
