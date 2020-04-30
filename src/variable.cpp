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

bool VariableContainer::setBoolVariable(std::string_view id, bool value) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        return false;
    }
    auto *var = dynamic_cast<BooleanVariable *>(variable_map[id].get());
    if (!var) {
        return false;
    }
    var->value = value;
    return true;
}

bool VariableContainer::setIntVariable(std::string_view id, int value) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        return false;
    }
    auto *var = dynamic_cast<IntegerVariable *>(variable_map[id].get());
    if (!var) {
        return false;
    }
    var->value = value;
    return true;
}

bool VariableContainer::setPlayerVariable(std::string_view id, Player *value) {
    auto iter = variable_map.find(id);
    if (iter == variable_map.end()) {
        return false;
    }
    auto *var = dynamic_cast<PlayerVariable *>(variable_map[id].get());
    if (!var) {
        return false;
    }
    var->value = value;
    return true;
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
