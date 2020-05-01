#include "action.h"

#include "game.h"
#include "popup.h"

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
    Token *token = nullptr;
    if (on.empty()) {
        token = game.variables.getTokenVariable(variable);
    } else {
        token = game.variables.getPlayerVariable(on)->variables.getTokenVariable(variable);
    }
    if (!activated and !token->currentPath) {
        token->selectOption(pathIndex);
        activated = true;
    }
    if (token->currentPath) {
        return false;
    }
    activated = false;
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
    game.variables.setNodeVariable("current_node", current_node);
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

//ForInt
ForIntAction::ForIntAction(std::string_view variable, int stop, int step) : variable{variable}, stop{stop}, step{step} {
}

bool ForIntAction::execute(Game &game) {
    for (; game.variables.getIntVariable(variable) != stop; game.variables.setIntVariable(variable,
                                                                                          game.variables.getIntVariable(
                                                                                                  variable) + step)) {
        if (!internal_sequence->execute(game)) {
            return false;
        }
    }
    return true;
}

std::unique_ptr<IAction> ForIntAction::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view variable = doc_node.attribute("variable").value();
    int stop = doc_node.attribute("stop").as_int();
    int step = doc_node.attribute("step").as_int();
    auto return_action = std::make_unique<ForIntAction>(variable, stop, step);
    return_action->internal_sequence = std::move(Sequence::parse(doc_node.first_child(), game));
    return return_action;
}


// SetTurnOrderByMax
SetTurnOrderByMax::SetTurnOrderByMax(std::string_view from) : from{from} {
}

bool SetTurnOrderByMax::execute(Game &game) {
    std::vector<unsigned> &order_vec = game.player_order;
    for (unsigned i = 0; i < order_vec.size() - 1; i++) {
        unsigned max_index = i;
        for (unsigned j = i + 1; j < order_vec.size(); j++) {
            if (game.players[order_vec[j]]->variables.getIntVariable(from) >
                game.players[order_vec[max_index]]->variables.getIntVariable(from)) {
                max_index = j;
            }
        }
        unsigned temp = order_vec[i];
        order_vec[i] = order_vec[max_index];
        order_vec[max_index] = temp;
    }
    return true;
}

std::unique_ptr<IAction> SetTurnOrderByMax::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view from = doc_node.attribute("from").value();
    return std::make_unique<SetTurnOrderByMax>(from);
}

// NoticePrompt


// BooleanPrompt
BooleanPromptAction::BooleanPromptAction(std::string_view variable, std::string_view on) : variable{variable}, on{on} {
}

bool BooleanPromptAction::execute(Game &game) {
    if (!popup) {
        if (on.empty()) {
            popup = std::make_unique<BooleanPopup>(tp.getText(game), game.game_app->renderer, *game.prompt_font,
                                                   game.variables.getRawBoolVariable(variable));
        } else {
            popup = std::make_unique<BooleanPopup>(tp.getText(game), game.game_app->renderer, *game.prompt_font,
                                                   game.variables.getPlayerVariable(on)->variables.getRawBoolVariable(
                                                           variable));
        }
        game.current_popup = popup.get();
    }
    if (popup->done) {
        popup.reset();
        game.current_popup = nullptr;
        return true;
    }
    return false;
}

std::unique_ptr<IAction> BooleanPromptAction::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view variable = doc_node.attribute("variable").value();
    std::string_view on = doc_node.attribute("on").value();
    auto prompt_var = std::make_unique<BooleanPromptAction>(variable, on);
    for (auto current = doc_node.first_child(); current; current = current.next_sibling()) {
        prompt_var->tp.parse(current, game);
    }
    return prompt_var;
}

// Prompt helpers
TextForPrompt::TextForPrompt(std::string_view raw_text) : raw_text{raw_text} {
}

std::string TextForPrompt::getValue(Game &game) {
    return raw_text;
}

ReferenceForPrompt::ReferenceForPrompt(std::string_view variable, std::string_view on) : variable{variable}, on{on} {
}

std::string ReferenceForPrompt::getValue(Game &game) {
    if (on.empty()) {
        return std::to_string(game.variables.getIntVariable(variable));
    } else {
        return std::to_string(game.variables.getPlayerVariable(on)->variables.getIntVariable(variable));
    }
}

std::string TextParser::getText(Game &game) {
    std::string ret_str;
    for (auto &component : components) {
        ret_str += component->getValue(game);
    }
    return ret_str;
}

void TextParser::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view type = doc_node.name();
    if (type == "Text") {
        std::string_view raw_text = doc_node.attribute("value").value();
        components.resize(components.size() + 1);
        components[components.size() - 1] = std::make_unique<TextForPrompt>(raw_text);
    } else if (type == "Reference") {
        std::string_view variable = doc_node.attribute("variable").value();
        std::string_view on = doc_node.attribute("on").value();
        components.resize(components.size() + 1);
        components[components.size() - 1] = std::make_unique<ReferenceForPrompt>(variable, on);
    }
}

// NoticePrompt
NoticePromptAction::NoticePromptAction() = default;

bool NoticePromptAction::execute(Game &game) {
    if (!popup) {
        popup = std::make_unique<NoticePopup>(tp.getText(game), game.game_app->renderer, *game.prompt_font);
        game.current_popup = popup.get();
    }
    if (popup->done) {
        popup.reset();
        game.current_popup = nullptr;
        return true;
    }
    return false;
}

std::unique_ptr<IAction> NoticePromptAction::parse(pugi::xml_node doc_node, Game &game) {
    auto prompt_var = std::make_unique<NoticePromptAction>();
    for (auto current = doc_node.first_child(); current; current = current.next_sibling()) {
        prompt_var->tp.parse(current, game);
    }
    return prompt_var;
}

// IntegerPrompt
IntegerPromptAction::IntegerPromptAction(std::string_view variable, std::string_view on, int initial, int min,
                                         int max) : variable{variable}, on{on}, initial{initial}, min{min}, max{max} {
}

bool IntegerPromptAction::execute(Game &game) {
    if (!popup) {
        if (on.empty()) {
            popup = std::make_unique<IntegerPopup>(tp.getText(game), game.game_app->renderer, *game.prompt_font,
                                                   game.variables.getRawIntVariable(variable), initial, min, max);
        } else {
            popup = std::make_unique<IntegerPopup>(tp.getText(game), game.game_app->renderer, *game.prompt_font,
                                                   game.variables.getPlayerVariable(on)->variables.getRawIntVariable(
                                                           variable), initial, min, max);
        }
        game.current_popup = popup.get();
    }
    if (popup->done) {
        popup.reset();
        game.current_popup = nullptr;
        return true;
    }
    return false;
}

std::unique_ptr<IAction> IntegerPromptAction::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view variable = doc_node.attribute("variable").value();
    std::string_view on = doc_node.attribute("on").value();
    int min = doc_node.attribute("on").as_int();
    int initial = doc_node.attribute("on").as_int(min);
    int max = doc_node.attribute("on").as_int();
    auto prompt_var = std::make_unique<IntegerPromptAction>(variable, on, initial, min, max);
    for (auto current = doc_node.first_child(); current; current = current.next_sibling()) {
        prompt_var->tp.parse(current, game);
    }
    return prompt_var;
}

// TokenPrompt
TokenPromptAction::TokenPromptAction(std::string_view variable, std::string_view on) : variable{variable}, on{on} {
}

bool TokenPromptAction::execute(Game &game) {
    if (!popup) {
        token_reference.clear();
        for (auto &token : game.tokens) {
            token_reference.push_back(token.get());
        }
        if (on.empty()) {
            popup = std::make_unique<TokenPopup>(tp.getText(game), game.game_app->renderer, *game.prompt_font,
                                                 token_reference,
                                                 game.variables.getRawTokenVariable(variable));
        } else {
            popup = std::make_unique<TokenPopup>(tp.getText(game), game.game_app->renderer, *game.prompt_font,
                                                 token_reference,
                                                 game.variables.getPlayerVariable(on)->variables.getRawTokenVariable(
                                                         variable));
        }
        game.current_popup = popup.get();
    }
    if (popup->done) {
        popup.reset();
        game.current_popup = nullptr;
        return true;
    }
    return false;
}

std::unique_ptr<IAction> TokenPromptAction::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view variable = doc_node.attribute("variable").value();
    std::string_view on = doc_node.attribute("on").value();
    auto prompt_var = std::make_unique<TokenPromptAction>(variable, on);
    for (auto current = doc_node.first_child(); current; current = current.next_sibling()) {
        prompt_var->tp.parse(current, game);
    }
    return prompt_var;
}