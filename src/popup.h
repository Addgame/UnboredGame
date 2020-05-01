#ifndef UNBORED_GAME_POPUP_H
#define UNBORED_GAME_POPUP_H

#include <string_view>
#include <memory>
#include <SDL_render.h>
#include "texture.h"
#include "font.h"
#include "token.h"
#include "variable.h"

using std::unique_ptr;

class IPopup {
public:
    bool done = false;

    virtual ~IPopup() = default;

    virtual void pressUp() {}

    virtual void pressDown() {}

    virtual void pressLeft() {}

    virtual void pressRight() {}

    virtual void pressCenter() {}

    virtual void render(SDL_Renderer *renderer, int x, int y) = 0;
};

class NoticePopup : public IPopup {
private:
    unique_ptr<Texture> background;

public:
    NoticePopup(std::string_view text, SDL_Renderer *renderer, Font &font);

    void pressCenter() override;

    void render(SDL_Renderer *renderer, int x, int y) override;
};

class BooleanPopup : public IPopup {
private:
    unique_ptr<Texture> background;
    BooleanVariable *output_var;

public:
    BooleanPopup(std::string_view text, SDL_Renderer *renderer, Font &font, BooleanVariable *output_var);

    void pressLeft() override;

    void pressRight() override;

    void render(SDL_Renderer *renderer, int x, int y) override;
};

class IntegerPopup : public IPopup {
private:
    unique_ptr<Texture> background;
    unique_ptr<Texture> value_texture;
    Font &font;
    SDL_Renderer *renderer;
    SDL_Point value_loc;
    IntegerVariable *output_var;
    int value;
    int min, max;

    void updateValueImage();

public:
    IntegerPopup(std::string_view text, SDL_Renderer *renderer, Font &font, IntegerVariable *output_var,
                 int initial_value, int min = 0, int max = 0);

    void pressLeft() override;

    void pressRight() override;

    void pressCenter() override;

    void render(SDL_Renderer *sdlRenderer, int x, int y) override;
};

class SelectionIntegerPopup : public IPopup {
private:
    unique_ptr<Texture> background;
    unique_ptr<Texture> value_texture;
    Font &font;
    SDL_Point value_loc;
    int value;
    int min, max;
public:
    // If min and max are the same value then no limits on the value
    SelectionIntegerPopup(std::string_view text, SDL_Renderer *renderer, Font &font, int initial_value, int min = 0,
                          int max = 0);

    void updateValueImage(SDL_Renderer *renderer);

    void decrement(SDL_Renderer *renderer);

    void increment(SDL_Renderer *renderer);

    int getValue() const;

    void render(SDL_Renderer *renderer, int x, int y) override;
};

class TokenPopup : public IPopup {
private:
    unique_ptr<Texture> background;
    SDL_Point token_loc;
    vector<Token *> &tokens;
    unsigned selected = 0;
    TokenVariable *output_variable;

    void updateTokenImage();

public:
    TokenPopup(std::string_view text, SDL_Renderer *renderer, Font &font, vector<Token *> &tokens,
               TokenVariable *output_variable);

    void pressLeft() override;

    void pressRight() override;

    void pressCenter() override;

    void render(SDL_Renderer *renderer, int x, int y) override;
};

#endif //UNBORED_GAME_POPUP_H
