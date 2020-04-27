#include "font.h"

Font::Font(string_view filename, int ptsize) {
    font = TTF_OpenFont(filename.data(), ptsize);
}

Font::~Font() {
    TTF_CloseFont(font);
}

Surface *Font::renderBlended(string_view text, SDL_Color color) {
    return new Surface(TTF_RenderText_Blended(font, text.data(), color));
}

Surface *Font::renderBlendedWrapped(string_view text, SDL_Color color, Uint32 wrap_length) {
    return new Surface(TTF_RenderText_Blended_Wrapped(font, text.data(), color, wrap_length));
}

Texture *Font::renderBlended(SDL_Renderer *renderer, string_view text, SDL_Color color) {
    SDL_Surface *tempSurf = TTF_RenderText_Blended(font, text.data(), color);
    auto *texture = new Texture(renderer, tempSurf);
    SDL_FreeSurface(tempSurf);
    return texture;
}

Texture *Font::renderSolid(SDL_Renderer *renderer, string_view text, SDL_Color color) {
    SDL_Surface *tempSurf = TTF_RenderText_Solid(font, text.data(), color);
    auto *texture = new Texture(renderer, tempSurf);
    SDL_FreeSurface(tempSurf);
    return texture;
}