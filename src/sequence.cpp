#include "sequence.h"

void Sequence::execute(Game &game) {
    if (condition && !condition->evaluate(game)) {
        return;
    }
    for (auto &action : actions) {
        action->execute(game);
    }
}
