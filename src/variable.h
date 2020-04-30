#ifndef UNBORED_GAME_VARIABLE_H
#define UNBORED_GAME_VARIABLE_H

#include <string>
#include <map>
#include <memory>
#include <pugixml.hpp>

class Player;

class Token;

class VariableContainer;

/*
 * Variables must be listed as a specific type and must have attribute:
 *   name - str (the variable name) ---- prohibited names are: current_player and current_player_number
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

class VariableContainer {
    std::map<std::string_view, std::unique_ptr<Variable>> variable_map;

public:
    bool exists(std::string_view id);

    bool addVariable(Variable *variable);

    bool setBoolVariable(std::string_view id, bool value);

    bool setIntVariable(std::string_view id, int value);

    bool setPlayerVariable(std::string_view id, Player *value);
};

#endif //UNBORED_GAME_VARIABLE_H
