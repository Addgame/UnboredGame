#include "sequence.h"

void Sequence::execute(Game &game) {
    for (auto &action : actions) {
        action->execute(game);
    }
}
