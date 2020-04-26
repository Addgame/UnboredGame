#ifndef UNBORED_GAME_PARSER_H
#define UNBORED_GAME_PARSER_H

class GameParser;

#include <vector>
#include <pugixml.hpp>
#include "game.h"
#include "application.h"

#include <iostream>

using std::vector;

class GameParser {
private:
    inline static const string games_dir = "../games/";

    pugi::xml_document xml_doc;
    vector<GameMeta *> metas;

    Application &app;

    void clearMetas();

public:
    explicit GameParser(Application &app);

    ~GameParser();

    const vector<GameMeta *> &parseMetas();

    GameMeta *parseMeta(const string &dir_name);

    Game *parseGame(unsigned meta_index);
};

#endif //UNBORED_GAME_PARSER_H
