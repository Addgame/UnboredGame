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
#include "variable.h"
#include "player.h"
#include "condition.h"
#include "popup.h"
#include "sequence.h"

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

enum class GameState {
    INITIALIZE = 0,
    BEGIN_TURN = 1,
    END_TURN = 2
};

class Game {
public:
    GameMeta meta;
    uint8_t num_players;

    Application *game_app = nullptr;

    std::unique_ptr<Font> prompt_font = nullptr;

    GameState current_state = GameState::INITIALIZE;
    Sequence *current_sequence = nullptr;

    vector<unique_ptr<Texture>> backgrounds;
    unsigned current_bg = 0;

    vector<unique_ptr<Node>> nodes;
    vector<unique_ptr<Path>> paths;

    vector<unique_ptr<Token>> tokens;

    VariableContainer variables;

    ConditionContainer conditions;

    SequenceContainer sequences;

    vector<unique_ptr<Player>> players;
    vector<unsigned> player_order;
    unsigned current_player_in_order = 0;

    IPopup *current_popup = nullptr;

    explicit Game(GameMeta &gm, uint8_t num_players);

    void update();

    void parse(Application &app, pugi::xml_node node);
};

#endif //UNBORED_GAME_GAME_H
