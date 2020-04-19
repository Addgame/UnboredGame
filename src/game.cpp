#include "game.h"

GameMeta::GameMeta(const string &name, const string &version, const string &description, const string &instructions,
                   const string &dir_name) : name(name), version(version), description(description),
                                             instructions(instructions), dir_name(dir_name) {
}
