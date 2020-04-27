#ifndef UNBORED_GAME_FONT_H
#define UNBORED_GAME_FONT_H

#include <string_view>
#include <SDL_ttf.h>
#include "texture.h"
#include "surface.h"

using std::string_view;

class Font {
private:
    TTF_Font *font;

public:
    Font(string_view filename, int ptsize);

    ~Font();

    Surface *renderBlended(string_view text, SDL_Color color);

    Surface *renderBlendedWrapped(string_view text, SDL_Color color, Uint32 length);

    Texture *renderBlended(SDL_Renderer *renderer, string_view text, SDL_Color color);

    Texture *renderSolid(SDL_Renderer *renderer, string_view text, SDL_Color color);
};

#endif //UNBORED_GAME_FONT_H
