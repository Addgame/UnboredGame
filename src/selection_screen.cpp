#include "application.h"
#include "screen.h"

using std::make_unique;

class MetaAdapter {
    GameMeta &gm;
    unique_ptr<Texture> image;

    explicit MetaAdapter(GameMeta &gameMeta) : gm{gameMeta} {

    }
};

SelectionScreen::SelectionScreen(Application &app) : app{app}, offset{0} {
    background = make_unique<Texture>(app.renderer, "../assets/gameselection.png");
    scrollbar = make_unique<Texture>(app.renderer, "../assets/scrollbar.png");
    game_bg = make_unique<Texture>(app.renderer, "../assets/smallmenuoption.png");
    games_image = nullptr;
}

SelectionScreen::~SelectionScreen() = default;

void SelectionScreen::hide() {

}

void SelectionScreen::show() {
    std::cout << "showing selection screen" << std::endl;
    gp.parseMetas();
}

void SelectionScreen::handleEvent(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                offset -= 5;
                break;
            case SDLK_DOWN:
                offset += 5;
                break;
            case SDLK_ESCAPE:
                app.setScreen(new MenuScreen(app));
                break;
        }
    }
}

void SelectionScreen::update() {

}

void SelectionScreen::render() {
    SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
    SDL_RenderClear(app.renderer);
    background->render(app.renderer, 0, 0);
    scrollbar->render(app.renderer, 560, 142 + 4 * offset);
    game_bg->render(app.renderer, 103, 142 + offset);
    SDL_RenderPresent(app.renderer);
}