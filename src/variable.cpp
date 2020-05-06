#include <iostream>
#include "variable.h"

bool VariableContainer::exists(std::string_view id) {
    return variable_map.count(id);
}

bool VariableContainer::addVariable(Variable *variable) {
    auto iter = variable_map.find(variable->id);
    if (iter == variable_map.end()) {
        variable_map[variable->id] = std::unique_ptr<Variable>(variable);
        return true;
    }
    return false;
}

void VariableContainer::setBoolVariable(std::string_view id, bool value) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        throw std::runtime_error("Could not find variable by name");
    }
    auto *var = dynamic_cast<BooleanVariable *>(variable_map[id].get());
    if (!var) {
        throw std::runtime_error("Referenced variable is not a boolean");
    }
    var->value = value;
}

bool VariableContainer::getBoolVariable(std::string_view id) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        throw std::runtime_error("Could not find variable by name");
    }
    auto *var = dynamic_cast<BooleanVariable *>(variable_map[id].get());
    if (!var) {
        throw std::runtime_error("Referenced variable is not a boolean");
    }
    return var->value;
}

BooleanVariable *VariableContainer::getRawBoolVariable(std::string_view id) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        throw std::runtime_error("Could not find variable by name");
    }
    auto *var = dynamic_cast<BooleanVariable *>(variable_map[id].get());
    if (!var) {
        throw std::runtime_error("Referenced variable is not a boolean");
    }
    return var;
}

void VariableContainer::setIntVariable(std::string_view id, int value) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        throw std::runtime_error("Could not find variable by name");
    }
    auto *var = dynamic_cast<IntegerVariable *>(variable_map[id].get());
    if (!var) {
        throw std::runtime_error("Referenced variable is not a integer");
    }
    var->value = value;
}

int VariableContainer::getIntVariable(std::string_view id) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        throw std::runtime_error("Could not find variable by name");
    }
    auto *var = dynamic_cast<IntegerVariable *>(variable_map[id].get());
    if (!var) {
        throw std::runtime_error("Referenced variable is not a integer");
    }
    return var->value;
}

IntegerVariable *VariableContainer::getRawIntVariable(std::string_view id) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        throw std::runtime_error("Could not find variable by name");
    }
    auto *var = dynamic_cast<IntegerVariable *>(variable_map[id].get());
    if (!var) {
        throw std::runtime_error("Referenced variable is not a integer");
    }
    return var;
}

void VariableContainer::setPlayerVariable(std::string_view id, Player *value) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        throw std::runtime_error("Could not find variable by name");
    }
    auto *var = dynamic_cast<PlayerVariable *>(variable_map[id].get());
    if (!var) {
        throw std::runtime_error("Referenced variable is not a Player");
    }
    var->value = value;
}

Player *VariableContainer::getPlayerVariable(std::string_view id) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        throw std::runtime_error("Could not find variable by name");
    }
    auto *var = dynamic_cast<PlayerVariable *>(variable_map[id].get());
    if (!var) {
        throw std::runtime_error("Referenced variable is not a Player");
    }
    return var->value;
}

void VariableContainer::setTokenVariable(std::string_view id, Token *value) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        throw std::runtime_error("Could not find variable by name");
    }
    auto *var = dynamic_cast<TokenVariable *>(variable_map[id].get());
    if (!var) {
        throw std::runtime_error("Referenced variable is not a Token");
    }
    var->value = value;
}

Token *VariableContainer::getTokenVariable(std::string_view id) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        throw std::runtime_error("Could not find variable by name");
    }
    auto *var = dynamic_cast<TokenVariable *>(variable_map[id].get());
    if (!var) {
        throw std::runtime_error("Referenced variable is not a Token");
    }
    return var->value;
}

TokenVariable *VariableContainer::getRawTokenVariable(std::string_view id) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        throw std::runtime_error("Could not find variable by name");
    }
    auto *var = dynamic_cast<TokenVariable *>(variable_map[id].get());
    if (!var) {
        throw std::runtime_error("Referenced variable is not a Token");
    }
    return var;
}

void VariableContainer::setNodeVariable(std::string_view id, Node *value) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        throw std::runtime_error("Could not find variable by name");
    }
    auto *var = dynamic_cast<NodeVariable *>(variable_map[id].get());
    if (!var) {
        throw std::runtime_error("Referenced variable is not a Node");
    }
    var->value = value;
}

Node *VariableContainer::getNodeVariable(std::string_view id) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        throw std::runtime_error("Could not find variable by name");
    }
    auto *var = dynamic_cast<NodeVariable *>(variable_map[id].get());
    if (!var) {
        throw std::runtime_error("Referenced variable is not a Node");
    }
    return var->value;
}

Network *VariableContainer::getNetworkVariable(std::string_view id) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        throw std::runtime_error("Could not find variable by name");
    }
    auto *var = dynamic_cast<NetworkVariable *>(variable_map[id].get());
    if (!var) {
        throw std::runtime_error("Referenced variable is not a Network");
    }
    return &var->value;
}

void BooleanVariable::parse(pugi::xml_node doc_node, VariableContainer &container) {
    std::string_view id = doc_node.attribute("name").value();
    if (id.empty()) {
        throw std::runtime_error("No valid BooleanVariable name given");
    }
    bool initial = doc_node.attribute("initial").as_bool(false);
    if (!container.addVariable(new BooleanVariable(id, initial))) {
        throw std::runtime_error("Unable to add BooleanVariable");
    }
}

void IntegerVariable::parse(pugi::xml_node doc_node, VariableContainer &container) {
    std::string_view id = doc_node.attribute("name").value();
    if (id.empty()) {
        throw std::runtime_error("No valid IntegerVariable name given");
    }
    int initial = doc_node.attribute("initial").as_int(false);
    if (!container.addVariable(new IntegerVariable(id, initial))) {
        throw std::runtime_error("Unable to add IntegerVariable");
    }
}

void PlayerVariable::parse(pugi::xml_node doc_node, VariableContainer &container) {
    std::string_view id = doc_node.attribute("name").value();
    if (id.empty()) {
        throw std::runtime_error("No valid PlayerVariable name given");
    }
    if (!container.addVariable(new PlayerVariable(id, nullptr))) {
        throw std::runtime_error("Unable to add PlayerVariable");
    }
}

void TokenVariable::parse(pugi::xml_node doc_node, VariableContainer &container) {
    std::string_view id = doc_node.attribute("name").value();
    if (id.empty()) {
        throw std::runtime_error("No valid TokenVariable name given");
    }
    if (!container.addVariable(new TokenVariable(id, nullptr))) {
        throw std::runtime_error("Unable to add TokenVariable");
    }
}

void NodeVariable::parse(pugi::xml_node doc_node, VariableContainer &container) {
    std::string_view id = doc_node.attribute("name").value();
    if (id.empty()) {
        throw std::runtime_error("No valid NodeVariable name given");
    }
    if (!container.addVariable(new NodeVariable(id, nullptr))) {
        throw std::runtime_error("Unable to add NodeVariable");
    }
}

void NetworkVariable::parse(pugi::xml_node doc_node, VariableContainer &container) {
    std::string_view id = doc_node.attribute("name").value();
    if (id.empty()) {
        throw std::runtime_error("No valid NetworkVariable name given");
    }
    if (!container.addVariable(new NetworkVariable(id))) {
        throw std::runtime_error("Unable to add NetworkVariable");
    }
}

bool Network::contains(Node *node) {
    for (Node *iter_node : nodes) {
        if (iter_node == node) {
            return true;
        }
    }
    return false;
}

void Network::add(Node *node) {
    nodes.push_back(node);
}

void Network::remove(Node *node) {
    for (auto iter = nodes.begin(); iter < nodes.end(); iter++) {
        if (*iter == node) {
            nodes.erase(iter);
            return;
        }
    }

}

Node *Network::get(unsigned int index) {
    return nodes.at(index);
}
