#ifndef UNBORED_GAME_SCREEN_H
#define UNBORED_GAME_SCREEN_H

#include <memory>

#include "application.h"
#include "texture.h"
#include "font.h"
#include "parser.h"
#include "surface.h"
#include "popup.h"

using std::unique_ptr;

class IScreen {
public:
    virtual ~IScreen() = default;

    virtual void hide() = 0;

    virtual void show() = 0;

    virtual void handleEvent(SDL_Event &event) = 0;

    virtual void update() = 0;

    virtual void render() = 0;
};

class MenuScreen : public IScreen {
private:
    int counter, r_count, selected;
    Application &app;
    unique_ptr<Texture> background;
    unique_ptr<Texture> menu_option;
    unique_ptr<Texture> highlight_menu_option;
    unique_ptr<Font> font;
    unique_ptr<Texture> new_game_text;
    unique_ptr<Texture> new_game_highlighted;
    unique_ptr<Texture> continue_game_text;
    unique_ptr<Texture> continue_game_highlighted;
    unique_ptr<Texture> options_text;
    unique_ptr<Texture> options_highlighted;

public:
    explicit MenuScreen(Application &app);

    ~MenuScreen() override;

    void hide() override;

    void show() override;

    void handleEvent(SDL_Event &event) override;

    void update() override;

    void render() override;
};

class SelectionScreen : public IScreen {
private:
    Application &app;
    unique_ptr<GameParser> gp;
    unique_ptr<Font> prompt_font;
    unique_ptr<Font> name_font;
    unique_ptr<Font> small_font;
    unique_ptr<Texture> background;
    unique_ptr<Texture> scrollbar;
    unique_ptr<Surface> game_bg;
    unique_ptr<Texture> games_image;
    unique_ptr<Texture> game_highlight;
    unique_ptr<Surface> detail_bg;
    SDL_Rect render_rect;
    vector<unique_ptr<Texture>> detail_textures;
    const vector<GameMeta *> *metas;
    unique_ptr<SelectionIntegerPopup> num_players_popup;
    unsigned scrollbox_offset;
    unsigned selected;

    void updateSelect(int delta);

public:
    explicit SelectionScreen(Application &app);

    ~SelectionScreen() override;

    void hide() override;

    void show() override;

    void handleEvent(SDL_Event &event) override;

    void update() override;

    void render() override;
};

class GameScreen : public IScreen {
private:
    Application &app;
    unique_ptr<Game> game;
    unsigned update_counter;
public:
    explicit GameScreen(Application &app, Game *game_ptr);

    void hide() override;

    void show() override;

    void handleEvent(SDL_Event &event) override;

    void update() override;

    void render() override;
};

#endif //UNBORED_GAME_SCREEN_H
