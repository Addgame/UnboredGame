#include "sequence.h"

#include "game.h"
#include "action.h"

bool Sequence::execute(Game &game) {
    if (!evaluated) {
        if (condition && (condition->evaluate(game) != !invert_condition)) {
            return true;
        }
        evaluated = true;
    }
    for (; current_action < actions.size(); current_action++) {
        if (!actions[current_action]->execute(game)) {
            return false;
        }
    }
    // once finished with all actions, reset back to start
    evaluated = false;
    current_action = 0;
    return true;
}

// run for Sequence, If, Unless
std::unique_ptr<IAction> Sequence::parse(pugi::xml_node doc_node, Game &game) {
    std::unique_ptr<IAction> ret_action = std::make_unique<Sequence>();
    auto *seq = dynamic_cast<Sequence *>(ret_action.get());
    std::string_view node_name = doc_node.name();
    if (node_name == "Unless") {
        seq->invert_condition = true;
    }
    std::string_view condition_name = doc_node.attribute("condition").value();
    if (!condition_name.empty()) {
        seq->condition = game.conditions.getCondition(condition_name);
    }

    for (auto current = doc_node.first_child(); current; current = current.next_sibling()) {
        std::string_view action_name = current.name();
        unique_ptr<IAction> action_ptr;
        if (action_name == "Sequence" or action_name == "If" or action_name == "Unless") {
            action_ptr = Sequence::parse(current, game);
        } else if (action_name == "AddInt") {
            action_ptr = AddIntAction::parse(current, game);
        } else if (action_name == "SubInt") {
            action_ptr = SubIntAction::parse(current, game);
        } else if (action_name == "SetInt") {
            action_ptr = SetIntAction::parse(current, game);
        } else if (action_name == "SetBool") {
            action_ptr = SetBoolAction::parse(current, game);
        } else if (action_name == "Call") {
            action_ptr = CallAction::parse(current, game);
        } else if (action_name == "SetTokenPathByIndex") {
            action_ptr = SetTokenPathByIndexAction::parse(current, game);
        } else if (action_name == "SetTokenHidden") {
            action_ptr = SetTokenHiddenAction::parse(current, game);
        } else if (action_name == "AddToNetwork") {
            action_ptr = AddToNetworkAction::parse(current, game);
        } else if (action_name == "RunLandSequence") {
            action_ptr = RunLandSequenceAction::parse(current, game);
        } else if (action_name == "ForEachPlayer") {
            action_ptr = ForEachPlayerAction::parse(current, game);
        } else if (action_name == "ForInt") {
            action_ptr = ForIntAction::parse(current, game);
        } else if (action_name == "SetTurnOrderByMax") {
            action_ptr = SetTurnOrderByMax::parse(current, game);
        } else if (action_name == "BooleanPrompt") {
            action_ptr = BooleanPromptAction::parse(current, game);
        } else if (action_name == "NoticePrompt") {
            action_ptr = NoticePromptAction::parse(current, game);
        } else if (action_name == "IntegerPrompt") {
            action_ptr = IntegerPromptAction::parse(current, game);
        } else if (action_name == "TokenPrompt") {
            action_ptr = TokenPromptAction::parse(current, game);
        }
        seq->actions.resize(seq->actions.size() + 1);
        seq->actions[seq->actions.size() - 1] = std::move(action_ptr);

    }

    return ret_action;
}

bool SequenceContainer::executeSequence(Game &game, std::string_view id) {
    return sequence_map.at(id.data())->execute(game);
}

void SequenceContainer::parse(pugi::xml_node doc_node, Game &game) {
    std::string_view seq_id = doc_node.attribute("id").value();
    if (seq_id.empty()) {
        throw std::runtime_error("Invalid sequence ID given");
    }
    auto iter = sequence_map.find(seq_id.data());
    if (iter == sequence_map.end()) {
        sequence_map[seq_id.data()] = Sequence::parse(doc_node, game);
        return;
    }
    throw std::runtime_error("Condition already exists with name");
}
