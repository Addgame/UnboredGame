#include <iostream>

#include "token.h"

using std::make_unique;

Token::Token(SDL_Renderer *renderer, const string &image_filename, int x, int y) : image{renderer, image_filename} {
    currentPath = nullptr;
    rect.x = x;
    rect.y = y;
    rect.w = image.w;
    rect.h = image.h;
    pathTicks = 0;
}

void Token::setPath(Path *newPath) {
    currentPath = newPath;
    rect.x = currentPath->from->x;
    rect.y = currentPath->from->y;
    currentNode = currentPath->to;
    pathTicks = 0;
}

void Token::setNode(Node *newNode) {
    currentNode = newNode;
    rect.x = newNode->x;
    rect.y = newNode->y;
}

void Token::selectOption(int index) {
    if (!currentNode) {
        return;
    }
    if ((index - 1) >= currentNode->outPaths.size()) {
        return;
    }
    setPath(currentNode->outPaths[index - 1]);
}

void Token::tick() {
    if (currentPath) {
        pathTicks++;
        if (pathTicks >= currentPath->numTicks) {
            rect.x = currentPath->to->x;
            rect.y = currentPath->to->y;
            currentPath = nullptr;
        } else {
            currentPath->getLocation(pathTicks, rect.x, rect.y);
        }
    }
}

void Token::parse(pugi::xml_node doc_node, const string &dir_name, SDL_Renderer *renderer,
                  vector<unique_ptr<Token>> &token_vec,
                  vector<unique_ptr<Node>> &node_vec) {
    string image_name = doc_node.attribute("image").value();
    if (image_name.empty()) {
        throw std::runtime_error("Invalid image name given to Token");
    }
    int x, y;
    Node *start_node = nullptr;
    if (!doc_node.attribute("start").empty()) {
        string_view node_name = doc_node.attribute("start").value();
        start_node = Node::getNodeByName(node_vec, node_name);
        if (!start_node) {
            throw std::runtime_error("No valid node name given");
        }
        x = start_node->x;
        y = start_node->y;
    } else {
        if (node_vec.empty()) {
            x = 0;
            y = 0;
        } else {
            auto &first_node = node_vec.front();
            x = first_node->x;
            y = first_node->y;
            start_node = first_node.get();
        }
    }
    token_vec.push_back(std::make_unique<Token>(renderer, "../games/" + dir_name + "/assets/" + image_name, x, y));
    token_vec.back()->setNode(start_node);
}
