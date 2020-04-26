#ifndef UNBORED_GAME_GAME_H
#define UNBORED_GAME_GAME_H

struct GameMeta;

class Game;

#include <cstdint>
#include <string>
#include <vector>
#include <pugixml/src/pugixml.hpp>
#include "texture.h"
#include "application.h"
#include "token.h"

using std::string;
using std::vector;

struct GameMeta {
    string name;
    string version;
    string description;
    string instructions;

    string dir_name;

    uint8_t min_players, max_players;
};

class Game {
public:
    GameMeta meta;

    vector<unique_ptr<Texture>> backgrounds;
    unsigned current_bg = 0;

    vector<unique_ptr<Node>> nodes;
    vector<unique_ptr<Path>> paths;

    vector<unique_ptr<Token>> tokens;

    explicit Game(GameMeta &gm);

    void parse(Application &app, pugi::xml_node node);
};

#endif //UNBORED_GAME_GAME_H
