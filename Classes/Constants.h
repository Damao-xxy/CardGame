#ifndef Constants_h
#define Constants_h

#include <string>

// 卡牌尺寸
const float kCardWidth = 80.0f;
const float kCardHeight = 120.0f;

// 花色枚举
enum CardSuit {
    SUIT_CLUBS,    // 梅花
    SUIT_DIAMONDS, // 方块
    SUIT_HEARTS,   // 红桃
    SUIT_SPADES,   // 黑桃
    SUIT_COUNT
};

// 牌面枚举
enum CardRank {
    RANK_A,      // A
    RANK_2,        // 2
    RANK_3,        // 3
    RANK_4,        // 4
    RANK_5,        // 5
    RANK_6,        // 6
    RANK_7,        // 7
    RANK_8,        // 8
    RANK_9,        // 9
    RANK_10,       // 10
    RANK_J,     // J
    RANK_Q,    // Q
    RANK_K,     // K
    RANK_COUNT
};

// 卡牌状态
enum CardState {
    CARD_STATE_FACE_DOWN, // 背面朝上
    CARD_STATE_FACE_UP,   // 正面朝上
    CARD_STATE_REMOVED    // 已移除
};

// 游戏状态
enum GameState {
    GAME_STATE_INITIALIZING, // 初始化中
    GAME_STATE_PLAYING,      // 游戏中
    GAME_STATE_PAUSED,       // 暂停
    GAME_STATE_WIN,          // 胜利
    GAME_STATE_LOSE          // 失败
};

// 卡牌区域枚举
enum CardZone {
    CARD_ZONE_STACK,      // 牌堆区
    CARD_ZONE_PLAYFIELD,  // 游戏区
    CARD_ZONE_HAND        // 手牌区（预留）
};
// 获取花色名称
std::string getSuitName(CardSuit suit);

// 获取牌面名称
std::string getRankName(CardRank rank);

#endif
