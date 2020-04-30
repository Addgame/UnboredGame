#include "parser.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

GameParser::GameParser(Application &app) : app{app} {

}

GameParser::~GameParser() {
    clearMetas();
}

void GameParser::clearMetas() {
    for (auto *meta : metas) {
        delete meta;
    }
    metas.clear();
}

const vector<GameMeta *> &GameParser::parseMetas() {
    clearMetas();
    fs::path gamesDir{games_dir};
    if (!fs::is_directory(gamesDir)) {
        return metas;
    }
    for (auto &dir_entry : fs::directory_iterator(gamesDir)) {
        if (!fs::is_directory(dir_entry.status())) {
            continue;
        }
        string dir_name = dir_entry.path().filename().string();
        GameMeta *currentMeta = parseMeta(dir_name);
        if (currentMeta)
            metas.push_back(currentMeta);
    }
    return metas;
}

GameMeta *GameParser::parseMeta(const string &dir_name) {
    std::cout << "Beginning parse attempt on: " << dir_name << std::endl;
    pugi::xml_parse_result result = xml_doc.load_file((games_dir + dir_name + "/meta.xml").c_str());
    if (result.status != pugi::status_ok) {
        std::cout << "  Could not get parsing meta in \"" << dir_name << "\" to happen" << std::endl;
        return nullptr;
    }
    string name, version, description, instructions;
    uint8_t min_players, max_players;
    auto meta_node = xml_doc.child("Meta");
    name = meta_node.attribute("name").value();
    if (name.empty()) {
        std::cout << "  ERROR: No valid name given for game" << std::endl;
        return nullptr;
    }
    version = meta_node.attribute("version").value();
    if (version.empty()) {
        std::cout << "  ERROR: No valid version given for game" << std::endl;
        return nullptr;
    }
    description = meta_node.child_value("Description");
    instructions = meta_node.child_value("Instructions");
    min_players = meta_node.child("Players").attribute("min").as_int();
    if (min_players < 1) {
        std::cout << "  ERROR: No valid min players given for game" << std::endl;
        return nullptr;
    }
    max_players = meta_node.child("Players").attribute("max").as_int();
    if (max_players < 1) {
        std::cout << "  ERROR: No valid max players given for game" << std::endl;
    }
    std::cout << "  Parsing directory successful: " << dir_name << std::endl;
    return new GameMeta{name, version, description, instructions, dir_name, min_players, max_players};
}

Game *GameParser::parseGame(unsigned meta_index, uint8_t num_players) {
    auto &meta_ref = *metas.at(meta_index);
    std::cout << "Parsing full game: " << meta_ref.dir_name << std::endl;
    pugi::xml_parse_result result = xml_doc.load_file((games_dir + meta_ref.dir_name + "/game.xml").c_str());
    if (result.status != pugi::status_ok) {
        std::cout << "  Could not get parsing game in \"" << meta_ref.dir_name << "\" to happen" << std::endl;
        return nullptr;
    }
    Game *game = new Game(meta_ref, num_players);
    try {
        game->parse(app, xml_doc.child("Game"));
        std::cout << "  Parsing game successful" << std::endl;
    } catch (std::exception &e) {
        std::cout << "  ERROR: " << e.what() << std::endl;
        delete game;
        game = nullptr;
    }
    return game;
}
