#include "application.h"
#include "screen.h"
#include "surface.h"

#include <sstream>

using std::make_unique;

SelectionScreen::SelectionScreen(Application &app) : app{app}, render_rect{0, 0, 448, 1}, metas{nullptr},
                                                     scrollbox_offset{0},
                                                     selected{0} {
    gp = make_unique<GameParser>(app);
    prompt_font = make_unique<Font>("../assets/Sen-Bold.ttf", 62);
    name_font = make_unique<Font>("../assets/Sen-Bold.ttf", 42);
    small_font = make_unique<Font>("../assets/Sen-Bold.ttf", 26);
    background = make_unique<Texture>(app.renderer, "../assets/gameselection.png");
    scrollbar = make_unique<Texture>(app.renderer, "../assets/scrollbar.png");
    game_bg = make_unique<Surface>(
            "../assets/smallmenuoption.png");
    game_highlight = make_unique<Texture>(app.renderer, "../assets/smallhighlightborder.png");
    detail_bg = make_unique<Surface>("../assets/gamedetailbg.png");
    games_image = nullptr;
}

SelectionScreen::~SelectionScreen() = default;

void SelectionScreen::hide() {

}

void SelectionScreen::show() {
    SDL_Color normal{178, 166, 166};
    SDL_Color light{200, 200, 200};
    SDL_Color white{255, 255, 255};
    // create texture for game list and textures for game detail
    auto &metasRef = gp->parseMetas();
    metas = &metasRef;
    unsigned total_height = std::max(104 * metasRef.size(), (unsigned long long) 1);
    //Surface full_surf = SDL_CreateRGBSurface(0, 448, static_cast<int>(total_height), 32, 0, 0, 0, 0);
    Surface full_surf = SDL_CreateRGBSurfaceWithFormat(0, 448, static_cast<int>(total_height), 32,
                                                       game_bg->surface->format->format);
    //SDL_SetColorKey(full_surf.surface, SDL_TRUE, SDL_MapRGB(full_surf.surface->format, 0xff, 0x00, 0xff));
    full_surf.fill(nullptr, 0, 0, 0, 0);
    for (unsigned i = 0; i < metasRef.size(); i++) {
        // game list texture
        full_surf.blit(*game_bg, 0, i * 104 + 2);
        auto name_surf = unique_ptr<Surface>(name_font->renderBlended(metasRef[i]->name, light));
        full_surf.blit(*name_surf, 19, i * 104 + 9);
        auto version_surf = unique_ptr<Surface>(small_font->renderBlended(metasRef[i]->version, light));
        full_surf.blit(*version_surf, 432 - version_surf->w, i * 104 + 24);
        auto dirname_surf = unique_ptr<Surface>(small_font->renderBlended(metasRef[i]->dir_name, normal));
        full_surf.blit(*dirname_surf, 19, i * 104 + 60);
        // game detail texture
        // 500 by 825
//        Surface detail_surf = SDL_ConvertSurfaceFormat(SDL_CreateRGBSurface(0, 500, 825, 32, 0, 0, 0, 0),
//                                                       SDL_PIXELFORMAT_RGBA8888, 0);
        // this pixel format allows per pixel alpha - I couldn't get it any other way
        //Surface detail_surf = SDL_CreateRGBSurfaceWithFormat(0, 500, 825, 32, game_bg->surface->format->format);
        Surface detail_surf = SDL_CreateRGBSurface(0, 512, 837, 32, 0, 0, 0, 0);
        //SDL_SetSurfaceAlphaMod(detail_surf.surface, 1);
        detail_surf.fill(nullptr, 255, 255, 255, 0);
        detail_surf.blit(*detail_bg, 0, 0);
        auto detail_name_surf = unique_ptr<Surface>(name_font->renderBlended(metasRef[i]->name, white));
        detail_surf.blit(*detail_name_surf, 6, 6);
        auto detail_version_surf = unique_ptr<Surface>(small_font->renderBlended(metasRef[i]->version, light));
        detail_surf.blit(*detail_version_surf, 504 - version_surf->w, 21);
        auto detail_dirname_surf = unique_ptr<Surface>(small_font->renderBlended(metasRef[i]->dir_name, light));
        detail_surf.blit(*detail_dirname_surf, 8, 57);
        try {
            auto detail_icon_surf = make_unique<Surface>("../games/" + metasRef[i]->dir_name + "/icon.png");
            detail_surf.blit(*detail_icon_surf, 16, 118);
        } catch (std::runtime_error &err) {
            std::cout << "Could not load an icon from game directory: " + metasRef[i]->dir_name << std::endl;
        }
        std::stringstream players_string;
        players_string << "Players: ";
        if (metasRef[i]->min_players == metasRef[i]->max_players) {
            players_string << static_cast<unsigned>(metasRef[i]->max_players);
        } else {
            players_string << static_cast<unsigned>(metasRef[i]->min_players) << "-"
                           << static_cast<unsigned>(metasRef[i]->max_players);
        }
        auto detail_players_surf = unique_ptr<Surface>(small_font->renderBlended(players_string.str(), light));
        detail_surf.blit(*detail_players_surf, 14, 397);
        auto detail_description_surf = unique_ptr<Surface>(
                small_font->renderBlendedWrapped(metasRef[i]->description, white, 484));
        detail_surf.blit(*detail_description_surf, 14, 427);
        detail_textures.push_back(make_unique<Texture>(app.renderer, detail_surf.surface));
    }
    games_image = make_unique<Texture>(app.renderer, full_surf.surface);
    render_rect.h = std::min((unsigned) 829, total_height);
}

void SelectionScreen::handleEvent(SDL_Event &event) {
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_UP:
                if (!num_players_popup)
                    updateSelect(-1);
                break;
            case SDLK_DOWN:
                if (!num_players_popup)
                    updateSelect(1);
                break;
            case SDLK_LEFT:
                if (num_players_popup)
                    num_players_popup->decrement(app.renderer);
                break;
            case SDLK_RIGHT:
                if (num_players_popup)
                    num_players_popup->increment(app.renderer);
                break;
            case SDLK_ESCAPE:
                app.setScreen(new MenuScreen(app));
                break;
            case SDLK_RETURN:
                int min_players = static_cast<int>(metas->at(selected)->min_players);
                int max_players = static_cast<int>(metas->at(selected)->max_players);
                if (num_players_popup || (min_players == max_players)) {
                    Game *game_ptr = gp->parseGame(selected,
                                                   num_players_popup ? num_players_popup->getValue() : min_players);
                    if (game_ptr) {
                        app.setScreen(new GameScreen(app, game_ptr));
                    } else {
                        std::cout << "Error in parsing game!" << std::endl;
                        num_players_popup.reset();
                    }
                } else {
                    if (metas->empty()) {
                        break;
                    }

                    num_players_popup = make_unique<SelectionIntegerPopup>("Number of Players", app.renderer,
                                                                           *prompt_font,
                                                                           min_players, min_players, max_players);
                }
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
    if (!metas->empty()) {
        game_highlight->render(app.renderer, 103, 104 * selected + 142 - render_rect.y);
        detail_textures[selected]->render(app.renderer, 670, 136);
    }
    if (num_players_popup) {
        num_players_popup->render(app.renderer, 240, 212);
    }
    SDL_RenderPresent(app.renderer);
}