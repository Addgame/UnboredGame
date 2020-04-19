#include <sstream>
#include "application.h"
#include "screen.h"

using std::make_unique;

// Menu Screen
MenuScreen::MenuScreen(Application &app) : counter{0}, app{app}, r_count{0}, selected{0}, background{nullptr} {
    background = make_unique<Texture>(app.renderer, "../assets/menubackground.png");
    menu_option = make_unique<Texture>(app.renderer, "../assets/menuoption2.png");
    highlight_menu_option = make_unique<Texture>(app.renderer, "../assets/highlightmenuoption.png");
    font = make_unique<Font>("../assets/Sen-Bold.ttf", 56);
    SDL_Color normal{178, 166, 166};
    SDL_Color highlight{214, 214, 214};
    new_game_text = unique_ptr<Texture>(font->renderBlended(app.renderer, "New Game", normal));
    new_game_highlighted = unique_ptr<Texture>(font->renderBlended(app.renderer, "New Game", highlight));
    continue_game_text = unique_ptr<Texture>(font->renderBlended(app.renderer, "Upload Game", normal));
    continue_game_highlighted = unique_ptr<Texture>(font->renderBlended(app.renderer, "Upload Game", highlight));
    options_text = unique_ptr<Texture>(font->renderBlended(app.renderer, "Options", normal));
    options_highlighted = unique_ptr<Texture>(font->renderBlended(app.renderer, "Options", highlight));
}

MenuScreen::~MenuScreen() = default;

void MenuScreen::hide() {
    std::cout << "hiding " << r_count << " FPS: " << r_count / (counter / 60) << std::endl;
}

void MenuScreen::show() {
    Application::log("showing menu screen");
}

void MenuScreen::handleEvent(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                selected = (selected - 1) % 3;
                if (selected < 0)
                    selected = 2;
                break;
            case SDLK_DOWN:
                selected = (selected + 1) % 3;
                break;
            case SDLK_RETURN:
                app.setScreen(new SelectionScreen(app));
                break;
        }
    }
}

void MenuScreen::update() {
    /*if (counter % 60 == 0) {
        std::stringstream s;
        s << "second " << (counter / 60);
        Application::log(s.str());
    }*/
    counter += 1;
}

void MenuScreen::render() {
    r_count += 1;
    SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
    SDL_RenderClear(app.renderer);
    background->render(app.renderer, 0, 0);
    if (selected != 0) {
        menu_option->render(app.renderer, 290, 350);
        new_game_text->render(app.renderer, 290 + (menu_option->w / 2) - (new_game_text->w / 2),
                              350 + (menu_option->h / 2) - (new_game_text->h / 2));
    } else {
        highlight_menu_option->render(app.renderer, 290, 350);
        new_game_highlighted->render(app.renderer, 290 + (menu_option->w / 2) - (new_game_text->w / 2),
                                     350 + (menu_option->h / 2) - (new_game_text->h / 2));
    }
    if (selected != 1) {
        menu_option->render(app.renderer, 290, 550);
        continue_game_text->render(app.renderer, 290 + menu_option->w / 2 - continue_game_text->w / 2,
                                   550 + menu_option->h / 2 - continue_game_text->h / 2);
    } else {
        highlight_menu_option->render(app.renderer, 290, 550);
        continue_game_highlighted->render(app.renderer, 290 + menu_option->w / 2 - continue_game_text->w / 2,
                                          550 + menu_option->h / 2 - continue_game_text->h / 2);
    }
    if (selected != 2) {
        menu_option->render(app.renderer, 290, 750);
        options_text->render(app.renderer, 290 + menu_option->w / 2 - options_text->w / 2,
                             750 + menu_option->h / 2 - options_text->h / 2);
    } else {
        highlight_menu_option->render(app.renderer, 290, 750);
        options_highlighted->render(app.renderer, 290 + menu_option->w / 2 - options_text->w / 2,
                                    750 + menu_option->h / 2 - options_text->h / 2);
    }
    SDL_RenderPresent(app.renderer);
}


// Game Screen
void GameScreen::hide() {

}

void GameScreen::show() {

}

void GameScreen::handleEvent(SDL_Event &event) {

}

void GameScreen::update() {

}

void GameScreen::render() {

}
