#ifndef UNBORED_GAME_VARIABLE_H
#define UNBORED_GAME_VARIABLE_H

#include <string>
#include <map>
#include <memory>
#include <pugixml.hpp>
#include <vector>

class Player;

class Token;

class Node;

class VariableContainer;

/*
 * Variables must be listed as a specific type and must have attribute:
 *   name - str (the variable name) ---- prohibited names are: current_player and current_player_number and current_node
 */
class Variable {
public:
    std::string id;

    explicit Variable(std::string_view id) : id{id} {}

    virtual ~Variable() = default;
};

class BooleanVariable : public Variable {
public:
    bool value;

    BooleanVariable(std::string_view id, bool initial) : value{initial}, Variable{id} {}

    ~BooleanVariable() override = default;

    static void parse(pugi::xml_node doc_node, VariableContainer &container);
};

class IntegerVariable : public Variable {
public:
    int value;

    IntegerVariable(std::string_view id, int initial) : value{initial}, Variable{id} {}

    ~IntegerVariable() override = default;

    static void parse(pugi::xml_node doc_node, VariableContainer &container);
};

class PlayerVariable : public Variable {
public:
    Player *value;

    PlayerVariable(std::string_view id, Player *initial) : value{initial}, Variable{id} {}

    ~PlayerVariable() override = default;

    static void parse(pugi::xml_node doc_node, VariableContainer &container);
};

class TokenVariable : public Variable {
public:
    Token *value;

    TokenVariable(std::string_view id, Token *initial) : value{initial}, Variable{id} {}

    ~TokenVariable() override = default;

    static void parse(pugi::xml_node doc_node, VariableContainer &container);
};

class NodeVariable : public Variable {
public:
    Node *value;

    NodeVariable(std::string_view id, Node *initial) : value{initial}, Variable{id} {}

    ~NodeVariable() override = default;

    static void parse(pugi::xml_node doc_node, VariableContainer &container);
};


class Network {
private:
    std::vector<Node *> nodes;
public:
    bool contains(Node *node);

    void add(Node *node);

    void remove(Node *node);

    Node *get(unsigned index);
};

class NetworkVariable : public Variable {
public:
    Network value;

    explicit NetworkVariable(std::string_view id) : Variable{id} {}

    ~NetworkVariable() override = default;

    static void parse(pugi::xml_node doc_node, VariableContainer &container);
};

class VariableContainer {
    std::map<std::string_view, std::unique_ptr<Variable>> variable_map;

public:
    bool exists(std::string_view id);

    bool addVariable(Variable *variable);

    void setBoolVariable(std::string_view id, bool value);

    bool getBoolVariable(std::string_view id);

    BooleanVariable *getRawBoolVariable(std::string_view id);

    void setIntVariable(std::string_view id, int value);

    int getIntVariable(std::string_view id);

    IntegerVariable *getRawIntVariable(std::string_view id);

    void setPlayerVariable(std::string_view id, Player *value);

    Player *getPlayerVariable(std::string_view id);

    void setTokenVariable(std::string_view id, Token *value);

    Token *getTokenVariable(std::string_view id);

    TokenVariable *getRawTokenVariable(std::string_view id);

    void setNodeVariable(std::string_view id, Node *value);

    Node *getNodeVariable(std::string_view id);

    Network *getNetworkVariable(std::string_view id);
};

#endif //UNBORED_GAME_VARIABLE_H
