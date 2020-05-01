#include "action.h"

#include "game.h"

// AddInt
AddIntAction::AddIntAction(std::string_view variable, std::string_view on, int value, std::string_view valueVar,
                           std::string_view valueOn) : variable{variable}, on{on}, value{value}, valueVar{valueVar},
                                                       valueOn{valueOn} {
}

bool AddIntAction::execute(Game &game) {
    int value_to_use = value;
    if (!valueVar.empty()) {
        if (valueOn.empty()) {
            value_to_use = game.variables.getIntVariable(valueVar);
        } else {
            value_to_use = game.variables.getPlayerVariable(valueOn)->variables.getIntVariable(valueVar);
        }
    }
    int original_value;
    if (on.empty()) {
        original_value = game.variables.getIntVariable(variable);
        game.variables.setIntVariable(variable, value_to_use + original_value);
    } else {
        original_value = game.variables.getPlayerVariable(on)->variables.getIntVariable(variable);
        game.variables.getPlayerVariable(on)->variables.setIntVariable(variable, value_to_use + original_value);
    }
    return true;
}

std::unique_ptr<IAction> AddIntAction::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view variable = doc_node.attribute("variable").value();
    std::string_view on = doc_node.attribute("on").value();
    int value = doc_node.attribute("value").as_int();
    std::string_view valueVar = doc_node.attribute("valueVar").value();
    std::string_view valueOn = doc_node.attribute("valueOn").value();
    return std::make_unique<AddIntAction>(variable, on, value, valueVar, valueOn);
}

// SubInt
SubIntAction::SubIntAction(std::string_view variable, std::string_view on, int value, std::string_view valueVar,
                           std::string_view valueOn) : variable{variable}, on{on}, value{value}, valueVar{valueVar},
                                                       valueOn{valueOn} {
}

bool SubIntAction::execute(Game &game) {
    int value_to_use = value;
    if (!valueVar.empty()) {
        if (valueOn.empty()) {
            value_to_use = game.variables.getIntVariable(valueVar);
        } else {
            value_to_use = game.variables.getPlayerVariable(valueOn)->variables.getIntVariable(valueVar);
        }
    }
    int original_value;
    if (on.empty()) {
        original_value = game.variables.getIntVariable(variable);
        game.variables.setIntVariable(variable, original_value - value_to_use);
    } else {
        original_value = game.variables.getPlayerVariable(on)->variables.getIntVariable(variable);
        game.variables.getPlayerVariable(on)->variables.setIntVariable(variable, original_value - value_to_use);
    }
    return true;
}

std::unique_ptr<IAction> SubIntAction::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view variable = doc_node.attribute("variable").value();
    std::string_view on = doc_node.attribute("on").value();
    int value = doc_node.attribute("value").as_int();
    std::string_view valueVar = doc_node.attribute("valueVar").value();
    std::string_view valueOn = doc_node.attribute("valueOn").value();
    return std::make_unique<SubIntAction>(variable, on, value, valueVar, valueOn);
}

// SetInt
SetIntAction::SetIntAction(std::string_view variable, std::string_view on, int value, std::string_view valueVar,
                           std::string_view valueOn) : variable{variable}, on{on}, value{value}, valueVar{valueVar},
                                                       valueOn{valueOn} {
}

bool SetIntAction::execute(Game &game) {
    int value_to_use = value;
    if (!valueVar.empty()) {
        if (valueOn.empty()) {
            value_to_use = game.variables.getIntVariable(valueVar);
        } else {
            value_to_use = game.variables.getPlayerVariable(valueOn)->variables.getIntVariable(valueVar);
        }
    }
    if (on.empty()) {
        game.variables.setIntVariable(variable, value_to_use);
    } else {
        game.variables.getPlayerVariable(on)->variables.setIntVariable(variable, value_to_use);
    }
    return true;
}

std::unique_ptr<IAction> SetIntAction::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view variable = doc_node.attribute("variable").value();
    std::string_view on = doc_node.attribute("on").value();
    int value = doc_node.attribute("value").as_int();
    std::string_view valueVar = doc_node.attribute("valueVar").value();
    std::string_view valueOn = doc_node.attribute("valueOn").value();
    return std::make_unique<SetIntAction>(variable, on, value, valueVar, valueOn);
}

// SetBool
SetBoolAction::SetBoolAction(std::string_view variable, std::string_view on, bool value, std::string_view valueVar,
                             std::string_view valueOn) : variable{variable}, on{on}, value{value}, valueVar{valueVar},
                                                         valueOn{valueOn} {
}

bool SetBoolAction::execute(Game &game) {
    bool value_to_use = value;
    if (!valueVar.empty()) {
        if (valueOn.empty()) {
            value_to_use = game.variables.getBoolVariable(valueVar);
        } else {
            value_to_use = game.variables.getPlayerVariable(valueOn)->variables.getBoolVariable(valueVar);
        }
    }
    if (on.empty()) {
        game.variables.setBoolVariable(variable, value_to_use);
    } else {
        game.variables.getPlayerVariable(on)->variables.setBoolVariable(variable, value_to_use);
    }
    return true;
}

std::unique_ptr<IAction> SetBoolAction::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view variable = doc_node.attribute("variable").value();
    std::string_view on = doc_node.attribute("on").value();
    bool value = doc_node.attribute("value").as_bool();
    std::string_view valueVar = doc_node.attribute("valueVar").value();
    std::string_view valueOn = doc_node.attribute("valueOn").value();
    return std::make_unique<SetBoolAction>(variable, on, value, valueVar, valueOn);
}

// Call
CallAction::CallAction(std::string_view id) : id{id} {

}

bool CallAction::execute(Game &game) {
    return game.sequences.executeSequence(game, id);
}

std::unique_ptr<IAction> CallAction::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view id = doc_node.attribute("id").value();
    return std::make_unique<CallAction>(id);
}

// SetTokenPathByIndex
SetTokenPathByIndexAction::SetTokenPathByIndexAction(std::string_view variable, std::string_view on,
                                                     unsigned int pathIndex) : variable{variable}, on{on},
                                                                               pathIndex{pathIndex} {

}

bool SetTokenPathByIndexAction::execute(Game &game) {
    if (on.empty()) {
        game.variables.getTokenVariable(variable)->selectOption(pathIndex);
    } else {
        game.variables.getPlayerVariable(on)->variables.getTokenVariable(variable)->selectOption(pathIndex);
    }
    return true;
}

std::unique_ptr<IAction> SetTokenPathByIndexAction::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view variable = doc_node.attribute("variable").value();
    std::string_view on = doc_node.attribute("on").value();
    unsigned pathIndex = doc_node.attribute("pathIndex").as_uint();
    return std::make_unique<SetTokenPathByIndexAction>(variable, on, pathIndex);
}

// SetTokenHidden
SetTokenHiddenAction::SetTokenHiddenAction(std::string_view variable, std::string_view on,
                                           bool value) : variable{variable}, on{on}, value{value} {

}

bool SetTokenHiddenAction::execute(Game &game) {
    if (on.empty()) {
        game.variables.getTokenVariable(variable)->hidden = value;
    } else {
        game.variables.getPlayerVariable(on)->variables.getTokenVariable(variable)->hidden = value;
    }
    return true;
}

std::unique_ptr<IAction> SetTokenHiddenAction::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view variable = doc_node.attribute("variable").value();
    std::string_view on = doc_node.attribute("on").value();
    bool value = doc_node.attribute("pathIndex").as_bool();
    return std::make_unique<SetTokenHiddenAction>(variable, on, value);
}

// AddToNetwork
AddToNetworkAction::AddToNetworkAction(std::string_view variable, std::string_view on, std::string_view valueVar,
                                       std::string_view valueOn) : variable{variable}, on{on}, valueVar{valueVar},
                                                                   valueOn{valueOn} {
}

bool AddToNetworkAction::execute(Game &game) {
    Node *value_to_use = nullptr;
    if (!valueVar.empty()) {
        if (valueOn.empty()) {
            value_to_use = game.variables.getNodeVariable(valueVar);
        } else {
            value_to_use = game.variables.getPlayerVariable(valueOn)->variables.getNodeVariable(valueVar);
        }
    }
    if (on.empty()) {
        game.variables.getNetworkVariable(variable)->add(value_to_use);
    } else {
        game.variables.getPlayerVariable(on)->variables.getNetworkVariable(variable)->add(value_to_use);
    }
    return true;
}

std::unique_ptr<IAction> AddToNetworkAction::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view variable = doc_node.attribute("variable").value();
    std::string_view on = doc_node.attribute("on").value();
    std::string_view valueVar = doc_node.attribute("valueVar").value();
    std::string_view valueOn = doc_node.attribute("valueOn").value();
    return std::make_unique<AddToNetworkAction>(variable, on, valueVar, valueOn);
}

// RunLandSequence
RunLandSequenceAction::RunLandSequenceAction(std::string_view tokenVar, std::string_view on) : tokenVar{tokenVar},
                                                                                               on{on} {
}

bool RunLandSequenceAction::execute(Game &game) {
    Node *current_node = nullptr;
    if (on.empty()) {
        current_node = game.variables.getTokenVariable(tokenVar)->currentNode;
    } else {
        current_node = game.variables.getPlayerVariable(on)->variables.getTokenVariable(tokenVar)->currentNode;
    }
    if (!current_node) {
        return true;
    }
    std::string_view seq_id = current_node->land_sequence;
    return game.sequences.executeSequence(game, seq_id);
}

std::unique_ptr<IAction> RunLandSequenceAction::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view variable = doc_node.attribute("variable").value();
    std::string_view on = doc_node.attribute("on").value();
    return std::make_unique<RunLandSequenceAction>(variable, on);
}

//ForEachPlayer
ForEachPlayerAction::ForEachPlayerAction(std::string_view player_var) : player_var{player_var} {
}

bool ForEachPlayerAction::execute(Game &game) {
    for (; player_index < game.num_players; player_index++) {
        game.variables.setPlayerVariable(player_var, game.players[player_index].get());
        if (!internal_sequence->execute(game)) {
            return false;
        }
    }
    player_index = 0;
    return true;
}

std::unique_ptr<IAction> ForEachPlayerAction::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view player_var = doc_node.attribute("player_var").value();
    auto return_action = std::make_unique<ForEachPlayerAction>(player_var);
    return_action->internal_sequence = std::move(Sequence::parse(doc_node.first_child(), game));
    return return_action;
}