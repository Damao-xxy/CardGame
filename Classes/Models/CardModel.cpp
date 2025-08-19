#include "CardModel.h"
#include <sstream>

CardModel::CardModel(int cardId, CardSuit suit, CardRank rank, std::string color)
    : _cardId(cardId), _suit(suit), _rank(rank), _color(color),
      _state(CARD_STATE_FACE_DOWN), _isMovable(false),
      _positionX(0), _positionY(0) {}

int CardModel::getCardId() const {
    return _cardId;
}

CardSuit CardModel::getSuit() const {
    return _suit;
}

CardRank CardModel::getRank() const {
    return _rank;
}

std::string CardModel::getColor() const {
    return _color;
}
CardState CardModel::getState() const {
    return _state;
}

CardZone CardModel::getZone() const {
    return _zone;
}

void CardModel::setState(CardState state) {
    _state = state;
}

void CardModel::setZone(CardZone zone) {
    _zone = zone;
}

bool CardModel::isMovable() const {
    return _isMovable;
}

void CardModel::setMovable(bool movable) {
    _isMovable = movable;
}

float CardModel::getPositionX() const {
    return _positionX;
}

float CardModel::getPositionY() const {
    return _positionY;
}

void CardModel::setPosition(float x, float y) {
    _positionX = x;
    _positionY = y;
}

/**
 * 检查当前卡牌是否能与另一张卡牌匹配
 * 匹配规则：两张卡牌的点数差为1（无花色限制）
 * @param other 要比较的另一张卡牌
 * @return 符合匹配规则返回true，否则返回false
 */
bool CardModel::matches(const CardModel* other) const {
    // 若传入空指针，直接返回不匹配
    if (!other) return false;
    
    // 匹配条件：当前卡牌点数比另一张多1，或少1
    int rankDiff = _rank - other->_rank;
    return (rankDiff == 1) || (rankDiff == -1);
}

std::string CardModel::toString() const {
    std::stringstream ss;
    ss << getRankName(_rank) << " of " << getSuitName(_suit);
    return ss.str();
}
