#ifndef UNBORED_GAME_GAME_H
#define UNBORED_GAME_GAME_H

#include <cstdint>
#include <string>
#include <SDL.h>

using std::string;

class GameMeta {
public:
    string name;
    string version;
    string description;
    string instructions;

    uint8_t min_players, max_players;

    string dir_name;

    GameMeta(const string &name, const string &version, const string &description, const string &instructions,
             const string &filename);
};

class Game {
private:
    GameMeta *meta;
};

#endif //UNBORED_GAME_GAME_H
