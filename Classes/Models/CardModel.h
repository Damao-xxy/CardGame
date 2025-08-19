#ifndef CardModel_h
#define CardModel_h

#include "../Constants.h"
#include <string>

class CardModel {
private:
    int _cardId;          // 卡牌唯一ID
    CardSuit _suit;       // 花色
    CardRank _rank;       // 牌面
    CardState _state;     // 状态
    CardZone _zone;       //区域
    bool _isMovable;      // 是否可移动
    float _positionX;     // X坐标
    float _positionY;     // Y坐标
    std::string _color;   //颜色
    
public:
    // 构造函数
    CardModel(int cardId, CardSuit suit, CardRank rank, std::string color);
    
    // 获取卡牌ID
    int getCardId() const;
    
    // 获取花色
    CardSuit getSuit() const;
    
    std::string getColor() const;
    
    // 获取牌面
    CardRank getRank() const;
    
    // 获取卡牌状态
    CardState getState() const;
    
    // 获取卡牌区域
    CardZone getZone() const;
    
    // 设置卡牌状态
    void setState(CardState state);
    
    // 设置卡牌区域
    void setZone(CardZone zone);
    // 检查卡牌是否可移动
    bool isMovable() const;
    
    // 设置卡牌是否可移动
    void setMovable(bool movable);
    
    // 获取X坐标
    float getPositionX() const;
    
    // 获取Y坐标
    float getPositionY() const;
    
    // 设置位置
    void setPosition(float x, float y);
    
    // 卡牌是否匹配
    bool matches(const CardModel* other) const;
    
    // 转换为字符串表示
    std::string toString() const;
};

#endif
