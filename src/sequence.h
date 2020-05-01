#ifndef UNBORED_GAME_SEQUENCE_H
#define UNBORED_GAME_SEQUENCE_H

#include <vector>
#include <memory>

#include "action.h"
#include "condition.h"


using std::vector;
using std::unique_ptr;

class Game;

// special sequence ids are begin_turn, initialize, end_turn
class Sequence : public IAction {
public:
    ~Sequence() override = default;

    vector<unique_ptr<IAction>> actions;
    unsigned current_action = 0;
    ICondition *condition = nullptr;
    bool invert_condition = false;
    bool evaluated = false;

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

class SequenceContainer {
public:
    std::map<std::string, unique_ptr<IAction>> sequence_map;

    bool executeSequence(Game &game, std::string_view id);

    void parse(pugi::xml_node doc_node, Game &game);
};

#endif //UNBORED_GAME_SEQUENCE_H
