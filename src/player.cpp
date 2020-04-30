#include "player.h"

#include <iostream>

void Layout::parse(pugi::xml_node doc_node, uint8_t num_players) {
    locations.resize(num_players);
    pugi::xml_node layout_node;
    for (layout_node = doc_node.child("Layout"); layout_node; layout_node = layout_node.next_sibling("Layout")) {
        unsigned min = layout_node.attribute("min").as_uint(1);
        unsigned max = layout_node.attribute("max").as_uint(255);
        if (min <= num_players && num_players <= max) {
            break;
        }
    }
    std::string_view type;
    if (layout_node) {
        type = layout_node.attribute("type").value();
    } else {
        type = "clockwise";
    }
    if (type == "duel") {
        for (unsigned i = 0; i < num_players; i++) {
            locations[i] = static_cast<Side>(2 * (i % 2));
        }
    } else if (type == "clockwise") {
        for (unsigned i = 0; i < num_players; i++) {
            locations[i] = static_cast<Side>(i % 4);
        }
    } else if (type == "counterclockwise") {
        unsigned temp = 0;
        for (unsigned i = 0; i < num_players; i++) {
            locations[i] = static_cast<Side>(temp);
            if (temp == 0)
                temp = 4;
            temp--;
        }
    }
}

Player::Player(Side side, uint8_t player_number) : side{side}, player_number{player_number} {

}
