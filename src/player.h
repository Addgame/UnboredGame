#ifndef UNBORED_GAME_PLAYER_H
#define UNBORED_GAME_PLAYER_H

#include <cstdint>

#include "token.h"

enum class Side {
    MASTER,
    ACROSS,
    LEFT,
    RIGHT
};


class Player : public TokenOwner {
    Side side;
    uint8_t player_number;
};

#endif //UNBORED_GAME_PLAYER_H
