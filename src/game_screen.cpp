#include "application.h"
#include "screen.h"

GameScreen::GameScreen(Application &app, Game *game_ptr) : app{app}, update_counter{0} {
    game = unique_ptr<Game>(game_ptr);
}

void GameScreen::hide() {

}

void GameScreen::show() {
}

void GameScreen::handleEvent(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                app.setScreen(new MenuScreen(app));
                break;
            case SDLK_1:
                game->tokens[0]->selectOption(1);
                break;
            case SDLK_2:
                game->tokens[0]->selectOption(2);
                break;
            case SDLK_3:
                game->tokens[0]->selectOption(3);
                break;
            case SDLK_4:
                game->tokens[0]->selectOption(4);
                break;
        }
    }
}

void GameScreen::update() {
    if (update_counter == 0) {
        for (auto &token : game->tokens) {
            token->tick();
        }
    }
    update_counter %= 3;
}

void GameScreen::render() {
    Texture texture{app.renderer, "../assets/marker.png"};
    SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
    SDL_RenderClear(app.renderer);
    game->backgrounds[game->current_bg]->render(app.renderer, 0, 0);
    for (auto &node : game->nodes) {
        texture.render(app.renderer, node->x, node->y);
    }
    for (auto &token : game->tokens) {
        if (!token->hidden)
            token->image.render(app.renderer, token->rect);
    }
    SDL_RenderPresent(app.renderer);
}
