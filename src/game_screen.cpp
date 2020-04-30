#include "application.h"
#include "screen.h"

GameScreen::GameScreen(Application &app, Game *game_ptr) : app{app}, update_counter{0} {
    game = unique_ptr<Game>(game_ptr);
}

void GameScreen::hide() {

}

void GameScreen::show() {
    /*std::cout << "showing game screen " << game->meta.name << std::endl;*/
    /*std::cout << "number of paths: " << game->paths.size() << "  number of nodes: " << game->nodes.size() << std::endl;*/
    /*for (auto &path : game->paths) {
        std::cout << "Path: addr=" << &path << " from=" << path.from << " to=" << path.to << " speed=" << path.numTicks << std::endl;
    }
    std::cout << "-----------------" << std::endl;*/
    /*for (auto &node : game->nodes) {
        std::cout << "Node: addr=" << node.get() << " id=" << node->id << " x=" << node->x << " y=" << node->y
                  << " outPaths=" << &(node->outPaths.at(0)) << std::endl;
        for (Path *p : node->outPaths) {
            std::cout << "   path*=" << p << " " << std::endl;
        }
    }*/
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
        token->image.render(app.renderer, token->rect);
    }
    SDL_RenderPresent(app.renderer);
}
