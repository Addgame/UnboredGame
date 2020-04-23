#include "application.h"
#include "screen.h"
#include "surface.h"

using std::make_unique;

SelectionScreen::SelectionScreen(Application &app) : app{app}, render_rect{0, 0, 448, 1}, metas{nullptr},
                                                     scrollbox_offset{0},
                                                     selected{0} {
    name_font = make_unique<Font>("../assets/Sen-Bold.ttf", 42);
    small_font = make_unique<Font>("../assets/Sen-Bold.ttf", 26);
    background = make_unique<Texture>(app.renderer, "../assets/gameselection.png");
    scrollbar = make_unique<Texture>(app.renderer, "../assets/scrollbar.png");
    game_bg = make_unique<Surface>(
            "../assets/smallmenuoption.png"); // make_unique<Texture>(app.renderer, "../assets/smallmenuoption.png");
    game_highlight = make_unique<Texture>(app.renderer, "../assets/smallhighlightborder.png");
    games_image = nullptr;
}

SelectionScreen::~SelectionScreen() = default;

void SelectionScreen::hide() {

}

void SelectionScreen::show() {
    SDL_Color normal{178, 166, 166};
    SDL_Color light{200, 200, 200};
    auto &metasRef = gp.parseMetas();
    metas = &metasRef;
    unsigned total_height = 104 * metasRef.size();
    Surface full_surf = SDL_CreateRGBSurface(0, 448, total_height, 32, 0, 0, 0, 0);
    SDL_SetColorKey(full_surf.surface, SDL_TRUE, SDL_MapRGB(full_surf.surface->format, 0xff, 0x00, 0xff));
    full_surf.fill(nullptr, 255, 0, 255, 255);
    for (unsigned i = 0; i < metasRef.size(); i++) {
        full_surf.blit(*game_bg, 0, i * 104 + 2);
        auto name_surf = make_unique<Surface>(*name_font->renderBlended(metasRef[i]->name, light));
        full_surf.blit(*name_surf, 19, i * 104 + 9);
        auto version_surf = make_unique<Surface>(*small_font->renderBlended(metasRef[i]->version, light));
        full_surf.blit(*version_surf, 432 - version_surf->w, i * 104 + 24);
        auto dirname_surf = make_unique<Surface>(*small_font->renderBlended(metasRef[i]->dir_name, normal));
        full_surf.blit(*dirname_surf, 19, i * 104 + 60);
    }
    games_image = make_unique<Texture>(app.renderer, full_surf.surface);
    render_rect.h = std::min((unsigned) 829, total_height);
}

void SelectionScreen::handleEvent(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                updateSelect(-1);
                break;
            case SDLK_DOWN:
                updateSelect(1);
                break;
            case SDLK_ESCAPE:
                app.setScreen(new MenuScreen(app));
                break;
            case SDLK_RETURN:

                app.setScreen(new GameScreen(app));
                break;
        }
    }
}

void SelectionScreen::updateSelect(int delta) {
    if (metas->empty()) {
        return;
    }
    unsigned new_value = std::max(std::min(static_cast<int>(delta + selected), static_cast<int>(metas->size()) - 1), 0);
    if (selected == new_value) {
        return;
    }
    selected = new_value;
    if ((selected + 1) * 104 - render_rect.y > 829) {
        // scroll down
        render_rect.y = std::min(render_rect.y + 104, games_image->h - 829);
    } else if (selected * 104 < render_rect.y) {
        // scroll up
        render_rect.y = std::max(0, render_rect.y - 104);
    }
    // when 0 or 1 meta then this will not be run (earlier return will catch that case)
    scrollbox_offset = static_cast<unsigned>(766 * selected / (metas->size() - 1));
}

void SelectionScreen::update() {

}

void SelectionScreen::render() {
    SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
    SDL_RenderClear(app.renderer);
    background->render(app.renderer, 0, 0);
    scrollbar->render(app.renderer, 560, 142 + scrollbox_offset);
    games_image->render(app.renderer, render_rect, 103, 140);
    game_highlight->render(app.renderer, 103, 104 * selected + 142 - render_rect.y);
    SDL_RenderPresent(app.renderer);
}