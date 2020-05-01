#include "game.h"
#include "condition.h"

#include <iostream>

Game::Game(GameMeta &gm, uint8_t num_players) : meta{gm}, num_players{num_players}, conditions{*this} {

}

void Game::parse(Application &app, pugi::xml_node node) {
    if (!node) {
        throw std::runtime_error("No valid Game node given");
    }
    // Get backgrounds
    for (auto current = node.child("Background"); current; current = current.next_sibling("Background")) {
        string_view sv = current.attribute("image").value();
        if (sv.empty()) {
            throw std::runtime_error("No valid \"image\" name given for Background");
        }
        backgrounds.push_back(
                std::make_unique<Texture>(app.renderer, "../games/" + meta.dir_name + "/assets/" + sv.data()));
    }
    if (backgrounds.empty()) {
        backgrounds.push_back(std::make_unique<Texture>(app.renderer, "../assets/defaultbg.png"));
    }
    // Get nodes
    for (auto current = node.child("Nodes").child("Node"); current; current = current.next_sibling("Node")) {
        nodes.push_back(Node::parse(current, nodes));
    }
    // Get paths
    for (auto current = node.child("Paths").child("Path"); current; current = current.next_sibling("Path")) {
        Path::parse(current, nodes, paths);
    }
    // Get tokens
    for (auto current = node.child("Globals").child("Token"); current; current = current.next_sibling("Token")) {
        Token::parse(current, meta.dir_name, app.renderer, tokens, nodes);
    }
    // Add system variables then get variables
    variables.addVariable(new PlayerVariable("current_player", nullptr));
    variables.addVariable(new IntegerVariable("current_player_number", 0));
    for (auto current = node.child("Globals").child("BooleanVariable"); current; current = current.next_sibling(
            "BooleanVariable")) {
        BooleanVariable::parse(current, variables);
    }
    for (auto current = node.child("Globals").child("IntegerVariable"); current; current = current.next_sibling(
            "IntegerVariable")) {
        IntegerVariable::parse(current, variables);
    }
    for (auto current = node.child("Globals").child("PlayerVariable"); current; current = current.next_sibling(
            "PlayerVariable")) {
        PlayerVariable::parse(current, variables);
    }
    for (auto current = node.child("Globals").child("TokenVariable"); current; current = current.next_sibling(
            "TokenVariable")) {
        TokenVariable::parse(current, variables);
    }
    for (auto current = node.child("Globals").child("NodeVariable"); current; current = current.next_sibling(
            "NodeVariable")) {
        NodeVariable::parse(current, variables);
    }
    for (auto current = node.child("Globals").child("NetworkVariable"); current; current = current.next_sibling(
            "NetworkVariable")) {
        NetworkVariable::parse(current, variables);
    }
    // Get layout then add players
    Layout layout;
    layout.parse(node.child("Layouts"), num_players);
    for (unsigned i = 0; i < num_players; i++) {
        players.emplace_back(std::make_unique<Player>(layout.locations[i], i));
    }
    // Get player data
    for (auto current = node.child("Players").child("BooleanVariable"); current; current = current.next_sibling(
            "BooleanVariable")) {
        for (auto &player : players) {
            BooleanVariable::parse(current, player->variables);
        }
    }
    for (auto current = node.child("Players").child("IntegerVariable"); current; current = current.next_sibling(
            "IntegerVariable")) {
        for (auto &player : players) {
            IntegerVariable::parse(current, player->variables);
        }
    }
    for (auto current = node.child("Players").child("PlayerVariable"); current; current = current.next_sibling(
            "PlayerVariable")) {
        for (auto &player : players) {
            PlayerVariable::parse(current, player->variables);
        }
    }
    for (auto current = node.child("Players").child("NodeVariable"); current; current = current.next_sibling(
            "NodeVariable")) {
        for (auto &player : players) {
            NodeVariable::parse(current, player->variables);
        }
    }
    for (auto current = node.child("Players").child("NetworkVariable"); current; current = current.next_sibling(
            "NetworkVariable")) {
        for (auto &player : players) {
            NetworkVariable::parse(current, player->variables);
        }
    }
    // Get conditions
    for (auto current = node.child("Conditions").first_child(); current; current = current.next_sibling()) {
        ConditionContainer::parse(current, *this);
    }
}
