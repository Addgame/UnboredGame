#ifndef UNBORED_GAME_ACTION_H
#define UNBORED_GAME_ACTION_H

#include <string_view>
#include <string>
#include <memory>
#include <pugixml.hpp>

class Game;

class Sequence;

class IAction {
public:
    virtual ~IAction() = default;

    // bool return is if action is done
    virtual bool execute(Game &game) = 0;
};

// variable - to be altered, on - if on player, value - if static value, valueVar - take from, valueOn - if valueVar on player
class AddIntAction : public IAction {
private:
    std::string variable;
    std::string on;
    int value;
    std::string valueVar;
    std::string valueOn;
public:
    AddIntAction(std::string_view variable, std::string_view on, int value, std::string_view valueVar,
                 std::string_view valueOn);

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

// variable - to be altered, on - if on player, value - if static value, valueVar - take from, valueOn - if valueVar on player
class SubIntAction : public IAction {
private:
    std::string variable;
    std::string on;
    int value;
    std::string valueVar;
    std::string valueOn;
public:
    SubIntAction(std::string_view variable, std::string_view on, int value, std::string_view valueVar,
                 std::string_view valueOn);

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

// variable - to be altered, on - if on player, value - if static value, valueVar - take from, valueOn - if valueVar on player
class SetIntAction : public IAction {
private:
    std::string variable;
    std::string on;
    int value;
    std::string valueVar;
    std::string valueOn;
public:
    SetIntAction(std::string_view variable, std::string_view on, int value, std::string_view valueVar,
                 std::string_view valueOn);

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

// variable - to be altered, on - if on player, value - if static value, valueVar - take from, valueOn - if valueVar on player
class SetBoolAction : public IAction {
private:
    std::string variable;
    std::string on;
    bool value;
    std::string valueVar;
    std::string valueOn;
public:
    SetBoolAction(std::string_view variable, std::string_view on, bool value, std::string_view valueVar,
                  std::string_view valueOn);

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

// id - sequence to call
class CallAction : public IAction {
private:
    std::string id;
public:
    explicit CallAction(std::string_view id);

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

// variable - string, on - if on player, pathIndex - int
class SetTokenPathByIndexAction : public IAction {
private:
    std::string variable;
    std::string on;
    unsigned pathIndex;
public:
    SetTokenPathByIndexAction(std::string_view variable, std::string_view on, unsigned pathIndex);

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

// variable - string, on - if on player, value - bool
class SetTokenHiddenAction : public IAction {
private:
    std::string variable;
    std::string on;
    bool value;
public:
    SetTokenHiddenAction(std::string_view variable, std::string_view on, bool value);

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

// variable - to be altered, on - if on player, valueVar - take from, valueOn - if valueVar on player
class AddToNetworkAction : public IAction {
private:
    std::string variable;
    std::string on;
    std::string valueVar;
    std::string valueOn;
public:
    AddToNetworkAction(std::string_view variable, std::string_view on, std::string_view valueVar,
                       std::string_view valueOn);

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

// tokenVar - string, on - if on player
class RunLandSequenceAction : public IAction {
private:
    std::string tokenVar;
    std::string on;
public:
    RunLandSequenceAction(std::string_view tokenVar, std::string_view on);

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

// player_var - string
class ForEachPlayerAction : public IAction {
private:
    std::unique_ptr<IAction> internal_sequence;
    std::string player_var;
    unsigned player_index = 0;
public:
    explicit ForEachPlayerAction(std::string_view player_var);

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

#endif //UNBORED_GAME_ACTION_H
