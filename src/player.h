#ifndef UNBORED_GAME_PLAYER_H
#define UNBORED_GAME_PLAYER_H

#include <cstdint>
#include <vector>

#include "variable.h"
#include "token.h"

enum class Side {
    MASTER = 0,
    LEFT = 1,
    ACROSS = 2,
    RIGHT = 3
};

/*
 * A Layout is located in the Layouts tag and has optional:
 *   min - int (minimum number of players for this layout) - defaults to 1
 *   max - int (maximum number of players for this layout) - defaults to 255 (max number of players)
 *   type - string ("duel", "clockwise", "counterclockwise") - defaults to "clockwise"
 * First valid Layout for the number of players will be selected
 */
class Layout {
public:
    vector<Side> locations;

    void parse(pugi::xml_node doc_node, uint8_t num_players);
};

class Player {
public:
    Side side;
    uint8_t player_number;
    VariableContainer variables;

    Player(Side side, uint8_t player_number);
};

#endif //UNBORED_GAME_PLAYER_H
