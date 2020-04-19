#include "texture.h"

#include <SDL_image.h>

Texture::Texture(SDL_Renderer *renderer, string_view filename, bool useColorKey) : w{0}, h{0} {
    SDL_Surface *tempSurf = IMG_Load(filename.data());
    if (useColorKey) {
        SDL_SetColorKey(tempSurf, SDL_TRUE, SDL_MapRGB(tempSurf->format, 0xff, 0x00, 0xff));
    }
    texture = SDL_CreateTextureFromSurface(renderer, tempSurf);
    SDL_FreeSurface(tempSurf);
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
}

Texture::Texture(SDL_Renderer *renderer, SDL_Surface *surf) : w{0}, h{0} {
    texture = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
}

Texture::~Texture() {
    SDL_DestroyTexture(texture);
}

bool Texture::valid() {
    return (texture != nullptr);
}

void Texture::render(SDL_Renderer *renderer, SDL_Rect &rect) {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

void Texture::render(SDL_Renderer *renderer, int x, int y) {
    SDL_Rect rect = {x, y, w, h};
    render(renderer, rect);
}
