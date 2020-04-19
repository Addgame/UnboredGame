#ifndef UNBORED_GAME_CONNECTIONS_H
#define UNBORED_GAME_CONNECTIONS_H

#include <vector>
#include <string>

using std::vector;
using std::string;

class Path;

class Node {
public:
    vector<Path *> outPaths;
    int x, y;
    string id;

    Node(const string &id, int x, int y);

    void addOutPath(Path *path);
};

class Path {
public:
    int dx, dy, numTicks;

    Node *from;
    Node *to;

    Path(Node &from, Node &to);

    Path(Node &from, Node &to, int speed);
};

#endif //UNBORED_GAME_CONNECTIONS_H
