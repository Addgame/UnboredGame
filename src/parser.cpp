#include "parser.h"

#include <filesystem>
#include <iostream>

#include "application.h"

namespace fs = std::filesystem;

GameParser::GameParser() = default;

GameParser::~GameParser() {
    clearMetas();
}

void GameParser::clearMetas() {
    for (auto meta : metas) {
        delete meta;
    }
    metas.clear();
}

const vector<GameMeta *> &GameParser::parseMetas() {
    clearMetas();
    fs::path gamesDir{games_dir};
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
    std::cout << "Attempt parse: " << dir_name << std::endl;
    pugi::xml_parse_result result = xml_doc.load_file((games_dir + dir_name + "/meta.xml").c_str());
    if (result.status != pugi::status_ok) {
        std::cout << "Could not get parsing meta in " << dir_name << "to happen" << std::endl;
        return nullptr;
    }
    string name, version, description, instructions;
    auto meta_node = xml_doc.child("Meta");
    name = meta_node.attribute("name").value();
    if (name.empty()) {
        std::cout << "ERROR: No valid name given for game" << std::endl;
        return nullptr;
    }
    version = meta_node.attribute("version").value();
    description = meta_node.child_value("Description");
    instructions = meta_node.child_value("Instructions");
    std::cout << "Parsing directory: " << dir_name << std::endl;
    return nullptr;
}

Game *GameParser::parseGame(const string &dir_name) {
    return nullptr;
}
