#ifndef UNBORED_GAME_CONDITION_H
#define UNBORED_GAME_CONDITION_H

#include <string_view>
#include <map>
#include <memory>
#include <pugixml.hpp>

class Game;

class ICondition {
public:
    std::string id;

    virtual bool evaluate(Game &game) = 0;
};

class ConditionContainer {
private:
    Game &game;
    std::map<std::string_view, std::unique_ptr<ICondition>> condition_map;
public:
    explicit ConditionContainer(Game &game);

    // the container will take over the pointer
    void addCondition(ICondition *condition);

    bool evaluate(std::string_view name);

    static void parse(pugi::xml_node doc_node, Game &game);
};

class BooleanCondition : public ICondition {
private:
    std::string variable_name;
    std::string on;
    bool invert;
public:
    BooleanCondition(Game &game, std::string_view variable_name, std::string_view on, bool invert);

    bool evaluate(Game &game) override;
};

class NetworkContainsCondition : public ICondition {
private:
    std::string network_name;
    std::string on;
    std::string has;
    std::string hasOn;
public:
    NetworkContainsCondition(Game &game, std::string_view network, std::string_view on, std::string_view has,
                             std::string_view hasOn);

    bool evaluate(Game &game) override;
};

#endif //UNBORED_GAME_CONDITION_H
