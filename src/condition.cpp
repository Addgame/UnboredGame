#include "condition.h"

#include "game.h"

void ConditionContainer::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view type = doc_node.name();
    std::string_view id = doc_node.attribute("id").value();
    if (type == "BooleanCondition") {
        std::string_view var_name = doc_node.attribute("variable").value();
        std::string_view on_value = doc_node.attribute("on").value();
        bool invert = doc_node.attribute("invert").as_bool(false);
        game.conditions.addCondition(new BooleanCondition(game, id, var_name, on_value, invert));
    } else if (type == "NetworkContainsCondition") {
        std::string_view network_name = doc_node.attribute("variable").value();
        std::string_view on_value = doc_node.attribute("on").value();
        std::string_view has_value = doc_node.attribute("has").value();
        std::string_view hasOn_value = doc_node.attribute("hasOn").value();
        game.conditions.addCondition(
                new NetworkContainsCondition(game, id, network_name, on_value, has_value, hasOn_value));
    } else {
        throw std::runtime_error("Invalid condition type");
    }
}

ConditionContainer::ConditionContainer(Game &game) : game{game} {}

void ConditionContainer::addCondition(ICondition *condition) {
    if (condition->id.empty()) {
        throw std::runtime_error("Invalid condition id");
    }
    auto iter = condition_map.find(condition->id);
    if (iter == condition_map.end()) {
        condition_map[condition->id] = std::unique_ptr<ICondition>(condition);
        return;
    }
    throw std::runtime_error("Condition already exists with name");
}

bool ConditionContainer::evaluate(std::string_view name) {
    return condition_map.at(name)->evaluate(game);
}

ICondition *ConditionContainer::getCondition(std::string_view id) {
    return condition_map.at(id).get();
}

BooleanCondition::BooleanCondition(Game &game, std::string_view id, std::string_view variable_name, std::string_view on,
                                   bool invert)
        : variable_name{variable_name}, on{on}, invert{invert}, ICondition{id} {
    if (on.empty()) {
        if (!game.variables.exists(variable_name)) {
            throw std::runtime_error("Variable name referenced in condition does not exist");
        }
    } else {
        if (!game.variables.exists(on)) {
            throw std::runtime_error("Player variable name referenced in condition does not exist");
        }
    }
}

bool BooleanCondition::evaluate(Game &game) {
    bool return_val;
    if (on.empty()) {
        return_val = game.variables.getBoolVariable(variable_name);
    } else {
        return_val = game.variables.getPlayerVariable(on)->variables.getBoolVariable(variable_name);
    }
    return return_val == !invert;
}

NetworkContainsCondition::NetworkContainsCondition(Game &game, std::string_view id, std::string_view network,
                                                   std::string_view on,
                                                   std::string_view has, std::string_view hasOn) : network_name{
        network}, on{on}, has{has}, hasOn{hasOn}, ICondition{id} {
    // could probably do parsing time checks here instead of just execution time
}

bool NetworkContainsCondition::evaluate(Game &game) {
    Node *toCheck;
    if (hasOn.empty()) {
        toCheck = game.variables.getNodeVariable(has);
    } else {
        toCheck = game.variables.getPlayerVariable(hasOn)->variables.getNodeVariable(has);
    }
    if (on.empty()) {
        return game.variables.getNetworkVariable(network_name)->contains(toCheck);
    } else {
        return game.variables.getPlayerVariable(on)->variables.getNetworkVariable(network_name)->contains(toCheck);
    }
}
