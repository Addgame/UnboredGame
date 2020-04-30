#include "popup.h"

using std::make_unique;

SelectionIntegerPopup::SelectionIntegerPopup(std::string_view text, SDL_Renderer *renderer, Font &font,
                                             int initial_value, int min, int max) : value{
        initial_value}, value_loc{0, 0}, font{font}, min{min}, max{max} {
    Surface bg_surf{"../assets/integerpopup.png"};
    SDL_Color white{255, 255, 255};
    auto text_surf = unique_ptr<Surface>(font.renderBlendedWrapped(text, white, 691));
    // center is 399.5, 203
    bg_surf.blit(*text_surf, 399 - text_surf->w / 2, 203 - text_surf->h / 2);
    background = make_unique<Texture>(renderer, bg_surf.surface);
    updateValueImage(renderer);
}

void SelectionIntegerPopup::updateValueImage(SDL_Renderer *renderer) {
    SDL_Color white{255, 255, 255};
    value_texture.reset(font.renderBlended(renderer, std::to_string(value), white));
    value_loc.x = 399 - value_texture->w / 2;
    value_loc.y = 431 - value_texture->h / 2;
}

void SelectionIntegerPopup::increment(SDL_Renderer *renderer) {
    if (min == max) {
        value++;
    } else {
        if (value >= max) {
            value = min;
        } else {
            value++;
        }
    }
    updateValueImage(renderer);
}

void SelectionIntegerPopup::decrement(SDL_Renderer *renderer) {
    if (min == max) {
        value--;
    } else {
        if (value <= min) {
            value = max;
        } else {
            value--;
        }
    }
    updateValueImage(renderer);
}

int SelectionIntegerPopup::getValue() const {
    return value;
}

void SelectionIntegerPopup::render(SDL_Renderer *renderer, int x, int y) {
    background->render(renderer, x, y);
    value_texture->render(renderer, x + value_loc.x, y + value_loc.y);
}

TokenPopup::TokenPopup(std::string_view text, SDL_Renderer *renderer, Font &font, vector<Token *> &tokens,
                       TokenVariable *output_variable) : tokens{tokens}, output_variable{output_variable},
                                                         token_loc{0, 0} {
    Surface bg_surf{"../assets/integerpopup.png"};
    SDL_Color white{255, 255, 255};
    auto text_surf = unique_ptr<Surface>(font.renderBlendedWrapped(text, white, 691));
    // center is 399.5, 203
    bg_surf.blit(*text_surf, 399 - text_surf->w / 2, 203 - text_surf->h / 2);
    background = make_unique<Texture>(renderer, bg_surf.surface);
    updateTokenImage();
}

void TokenPopup::updateTokenImage() {
    auto *current_token = tokens[selected];
    token_loc.x = 399 - current_token->image.w / 2;
    token_loc.y = 431 - current_token->image.h / 2;
}

void TokenPopup::pressLeft() {
    if (selected == 0) {
        selected = tokens.size();
    }
    selected--;
    updateTokenImage();
}

void TokenPopup::pressRight() {
    if (selected == tokens.size()) {
        selected = 0;
    } else {
        selected++;
    }
    updateTokenImage();
}

void TokenPopup::pressCenter() {
    if (output_variable) {
        output_variable->value = tokens[selected];
    }
    done = true;
}

void TokenPopup::render(SDL_Renderer *renderer, int x, int y) {
    background->render(renderer, x, y);
    tokens[selected]->image.render(renderer, x + token_loc.x, y + token_loc.y);
}
