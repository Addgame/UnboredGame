#include "game.h"

Game::Game(GameMeta &gm) : meta{gm} {

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
}
