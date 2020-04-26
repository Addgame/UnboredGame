#ifndef UNBORED_GAME_CONDITION_H
#define UNBORED_GAME_CONDITION_H

#include "game.h"

class ICondition {
    virtual bool evaluate(Game &game) = 0;
};

#endif //UNBORED_GAME_CONDITION_H
