#ifndef UNBORED_GAME_ACTION_H
#define UNBORED_GAME_ACTION_H

#include "game.h"

class IAction {
public:
    virtual void execute(Game &game) = 0;
};

#endif //UNBORED_GAME_ACTION_H
