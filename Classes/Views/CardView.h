#ifndef CardView_h
#define CardView_h

#include "cocos2d.h"
#include "../Models/CardModel.h"
#include <functional>

class CardView : public cocos2d::Sprite {
private:
    int _cardId;                          // 卡牌ID
    cocos2d::Sprite* _frontSprite;        // 正面精灵
    cocos2d::Sprite* _backSprite;         // 背面精灵
    std::function<void(int)> _clickCallback; // 点击回调
    Sprite* _suitSprite;   // 花色精灵
    Sprite* _rankSprite;   // 数字/字母精灵
    
public:
    // 创建卡牌视图
    static CardView* create(int cardId, CardSuit suit, CardRank rank, std::string color);
    
    // 构造函数
    CardView(int cardId);
    
    // 析构函数
    ~CardView() override;
    
    // 初始化
    bool init(CardSuit suit, CardRank rank, std::string color);
    
    // 设置点击回调
    void setClickCallback(const std::function<void(int)>& callback);
    
    // 更新视图以匹配模型状态
    void updateWithModel(const CardModel* model);
    
    // 翻转动画
    void flipCard(bool faceUp, float duration = 0.3f);
    
    // 移动动画
    void moveToPosition(float x, float y, float duration = 0.3f);
    
    // 移除动画
    void removeWithAnimation(float duration = 0.3f);
    
    int getCardId() const; 
    
private:
    // 触摸事件处理
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    // 设置卡牌纹理
    void setupTextures(CardSuit suit, CardRank rank, std::string color);
};

#endif
