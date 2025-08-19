#include "Constants.h"

std::string getSuitName(CardSuit suit) {
    switch (suit) {
        case SUIT_CLUBS: return "club";
        case SUIT_DIAMONDS: return "diamond";
        case SUIT_HEARTS: return "heart";
        case SUIT_SPADES: return "spade";
        default: return "unknown";
    }
}

std::string getRankName(CardRank rank) {
    switch (rank) {
        case RANK_A: return "A";
        case RANK_2: return "2";
        case RANK_3: return "3";
        case RANK_4: return "4";
        case RANK_5: return "5";
        case RANK_6: return "6";
        case RANK_7: return "7";
        case RANK_8: return "8";
        case RANK_9: return "9";
        case RANK_10: return "10";
        case RANK_J: return "J";
        case RANK_Q: return "Q";
        case RANK_K: return "K";
        default: return "unknown";
    }
}
