#include <iostream>
#include <string_view>
#include <cmath>

#include "connections.h"


Node::Node(const string &id, int x, int y, string_view land_sequence, string_view touch_sequence) : land_sequence{
        land_sequence},
                                                                                                    touch_sequence{
                                                                                                            touch_sequence} {
    this->id = id;
    this->x = x;
    this->y = y;
}

void Node::addOutPath(Path *path) {
    outPaths.push_back(path);
}

std::unique_ptr<Node> Node::parse(pugi::xml_node doc_node, vector<unique_ptr<Node>> &node_list) {
    // ID
    string node_id = doc_node.attribute("id").value();
    if (node_id.empty()) {
        throw std::runtime_error("No valid ID given to node");
    }
    if (getNodeByName(node_list, node_id)) {
        throw std::runtime_error("Node ID already exists");
    }
    // X
    string_view temp = doc_node.attribute("x").value();
    if (temp.empty()) {
        throw std::runtime_error("No valid x given to node");
    }
    int node_x = doc_node.attribute("x").as_int();
    // Y
    temp = doc_node.attribute("y").value();
    if (temp.empty()) {
        throw std::runtime_error("No valid y given to node");
    }
    int node_y = doc_node.attribute("y").as_int();

    return std::make_unique<Node>(node_id, node_x, node_y);
}

Node *Node::getNodeByName(vector<unique_ptr<Node>> &node_list, string_view name) {
    for (auto &current : node_list) {
        if (current->id == name) {
            return current.get();
        }
    }
    return nullptr;
}

Path::Path(Node &from, Node &to, int speed) {
    this->from = &from;
    this->to = &to;
    double distance = sqrt(pow(to.x - from.x, 2) + pow(to.y - from.y, 2));
    numTicks = std::max(1, static_cast<int>(distance / speed));
}

void Path::getLocation(int ticks, int &nextX, int &nextY) const {
    nextX = from->x + ticks * (to->x - from->x) / numTicks;
    nextY = from->y + ticks * (to->y - from->y) / numTicks;
}

void Path::parse(pugi::xml_node doc_node, vector<unique_ptr<Node>> &nodes, vector<unique_ptr<Path>> &paths) {
    // From Node
    string_view temp = doc_node.attribute("from").value();
    if (temp.empty()) {
        throw std::runtime_error("Path not given a from Node");
    }
    Node *from_node = Node::getNodeByName(nodes, temp);
    if (!from_node) {
        throw std::runtime_error("From Node name not valid");
    }
    // To Node
    temp = doc_node.attribute("to").value();
    if (temp.empty()) {
        throw std::runtime_error("Path not given a to Node");
    }
    Node *to_node = Node::getNodeByName(nodes, temp);
    if (!to_node) {
        throw std::runtime_error("To Node name not valid");
    }
    // Speed
    int speed = doc_node.attribute("speed").as_int(10);
    if (speed <= 0) {
        throw std::runtime_error("Path given a nonpositive speed");
    }
    // Add Path
    paths.push_back(std::make_unique<Path>(*from_node, *to_node, speed));
    from_node->addOutPath(paths.back().get());
    // If bidirectional then add Path back
    if (doc_node.attribute("bidirectional").as_bool(false)) {
        paths.push_back(std::make_unique<Path>(*to_node, *from_node, speed));
        to_node->addOutPath(paths.back().get());
    }
}

