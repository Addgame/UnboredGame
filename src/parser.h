#ifndef UNBORED_GAME_PARSER_H
#define UNBORED_GAME_PARSER_H

#include <vector>
#include <pugixml.hpp>
#include "game.h"

using std::vector;

class GameParser {
private:
    inline static const string games_dir = "../games/";

    pugi::xml_document xml_doc;
    vector<GameMeta *> metas;

    void clearMetas();

public:

    GameParser();

    ~GameParser();

    const vector<GameMeta *> &parseMetas();

    GameMeta *parseMeta(const string &dir_name);

    Game *parseGame(const string &dir_name);
};

#endif //UNBORED_GAME_PARSER_H
