#ifndef UNBORED_GAME_CONNECTIONS_H
#define UNBORED_GAME_CONNECTIONS_H

#include <vector>
#include <memory>
#include <string>
#include <pugixml/src/pugixml.hpp>

using std::vector;
using std::string;
using std::string_view;
using std::unique_ptr;

class Path;

/*
 * Node has required attributes id (string), x (int), y (int)
 */
class Node {
public:
    vector<Path *> outPaths;
    int x, y;
    string id;
    string land_sequence;
    string touch_sequence;

    Node(const string &id, int x, int y, string_view land_sequence = "", string_view touch_sequence = "");

    void addOutPath(Path *path);

    static Node *getNodeByName(vector<unique_ptr<Node>> &node_list, string_view name);

    static std::unique_ptr<Node> parse(pugi::xml_node doc_node, vector<unique_ptr<Node>> &node_list);
};

/*
 * Path has required attributes
 *  from (string) - Node id
 *  to (string) - Node id
 * And optional attributes
 *  speed (int) - defaults to 10
 *  bidirectional (bool) - defaults to false
 */
class Path {
public:
    int numTicks;

    Node *from;
    Node *to;

    Path(Node &from, Node &to, int speed);

    void getLocation(int ticks, int &nextX, int &nextY) const;

    static void parse(pugi::xml_node doc_node, vector<unique_ptr<Node>> &nodes, vector<unique_ptr<Path>> &paths);
};

#endif //UNBORED_GAME_CONNECTIONS_H
