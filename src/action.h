#ifndef UNBORED_GAME_ACTION_H
#define UNBORED_GAME_ACTION_H

#include <string_view>
#include <string>
#include <memory>
#include <pugixml.hpp>
#include <vector>

class Game;

class Sequence;

class BooleanPopup;

class IntegerPopup;

class NoticePopup;

class Token;

class TokenPopup;

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
class MultiplyIntAction : public IAction {
private:
    std::string variable;
    std::string on;
    int value;
    std::string valueVar;
    std::string valueOn;
public:
    MultiplyIntAction(std::string_view variable, std::string_view on, int value, std::string_view valueVar,
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
    bool activated = false;
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

// player_var - string
class ForIntAction : public IAction {
private:
    std::unique_ptr<IAction> internal_sequence;
    std::string variable;
    int stop, step;
public:
    explicit ForIntAction(std::string_view variable, int stop, int step);

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

// from - string for variable to sort by max
class SetTurnOrderByMax : public IAction {
private:
    std::string from;
public:
    explicit SetTurnOrderByMax(std::string_view from);

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

class IForPrompt {
public:
    virtual std::string getValue(Game &game) = 0;
};

class TextForPrompt : public IForPrompt {
private:
    std::string raw_text;
public:
    explicit TextForPrompt(std::string_view raw_text);

    std::string getValue(Game &game) override;
};

class ReferenceForPrompt : public IForPrompt {
private:
    std::string variable;
    std::string on;
public:
    ReferenceForPrompt(std::string_view variable, std::string_view on);

    std::string getValue(Game &game) override;
};

class TextParser {
private:
    std::vector<std::unique_ptr<IForPrompt>> components;
public:
    std::string getText(Game &game);

    void parse(pugi::xml_node doc_node, Game &game);
};

// variable - string, on - player if applicable
class BooleanPromptAction : public IAction {
private:
    std::string variable;
    std::string on;
    std::unique_ptr<BooleanPopup> popup;
    TextParser tp;
public:
    BooleanPromptAction(std::string_view variable, std::string_view on);

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

// no attributes
class NoticePromptAction : public IAction {
private:
    std::unique_ptr<NoticePopup> popup;
    TextParser tp;
public:
    explicit NoticePromptAction();

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

// variable - string, on - string for player if applicable, initial, min, max - 3 ints
class IntegerPromptAction : public IAction {
private:
    std::string variable;
    std::string on;
    std::unique_ptr<IntegerPopup> popup;
    TextParser tp;
    int initial, min, max;
public:
    IntegerPromptAction(std::string_view variable, std::string_view on, int initial, int min, int max);

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

// variable - string, on - string for player if applicable
class TokenPromptAction : public IAction {
private:
    std::string variable;
    std::string on;
    std::unique_ptr<TokenPopup> popup;
    std::vector<Token *> token_reference;
    TextParser tp;
public:
    TokenPromptAction(std::string_view variable, std::string_view on);

    bool execute(Game &game) override;

    static std::unique_ptr<IAction> parse(pugi::xml_node doc_node, Game &game);
};

#endif //UNBORED_GAME_ACTION_H
