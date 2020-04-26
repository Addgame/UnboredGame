#ifndef UNBORED_GAME_SEQUENCE_H
#define UNBORED_GAME_SEQUENCE_H

#include <vector>
#include <memory>

#include "action.h"
#include "condition.h"


using std::vector;
using std::unique_ptr;

class Sequence : public IAction {
public:
    vector<unique_ptr<IAction>> actions;
    ICondition *condition;

    void execute(Game &game) override;
};

class SequenceContainer {
public:
    vector<unique_ptr<Sequence>> sequences;
};

#endif //UNBORED_GAME_SEQUENCE_H
