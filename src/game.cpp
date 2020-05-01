#include "game.h"
#include "condition.h"

#include <iostream>

Game::Game(GameMeta &gm, uint8_t num_players) : meta{gm}, num_players{num_players}, conditions{*this} {
    for (unsigned i = 0; i < num_players; i++) {
        player_order.push_back(i);
    }
}

void Game::update() {
    if (current_state == GameState::INITIALIZE) {
        if (sequences.executeSequence(*this, "initialize")) {
            current_state = GameState::BEGIN_TURN;
        }
    } else if (current_state == GameState::BEGIN_TURN) {
        if (sequences.executeSequence(*this, "begin_turn")) {
            current_state = GameState::END_TURN;
        }
    } else {
        if (sequences.executeSequence(*this, "initialize")) {
            current_player_in_order++;
            variables.setIntVariable("current_player_index", player_order[current_player_in_order]);
            variables.setPlayerVariable("current_player", players[player_order[current_player_in_order]].get());
            current_state = GameState::BEGIN_TURN;
        }
    }
}

void Game::parse(Application &app, pugi::xml_node node) {
    if (!game_app) {
        game_app = &app;
        prompt_font = std::make_unique<Font>("../assets/Sen-Bold.ttf", 62);
    }
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
    // Get sequences
    for (auto current = node.child("Sequences").child("Sequence"); current; current = current.next_sibling()) {
        sequences.parse(current, *this);
    }
}
