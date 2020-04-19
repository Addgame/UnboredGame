#include <iostream>
#include <cmath>

#include "connections.h"

Node::Node(const string &id, int x, int y) {
    this->id = id;
    this->x = x;
    this->y = y;
}

void Node::addOutPath(Path *path) {
    outPaths.push_back(path);
}

Path::Path(Node &from, Node &to) : Path(from, to, 10) {}

Path::Path(Node &from, Node &to, int speed) {
    this->from = &from;
    this->to = &to;
    double distance = sqrt(pow(to.x - from.x, 2) + pow(to.y - from.y, 2));
    numTicks = static_cast<int>(distance / speed);
    std::cout << "distance: " << distance;
    std::cout << " num ticks: " << numTicks;
    dx = (to.x - from.x) / numTicks;
    dy = (to.y - from.y) / numTicks;
    std::cout << " dx: " << dx;
    std::cout << " dy: " << dy << std::endl;
}
