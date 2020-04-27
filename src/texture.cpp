#include "texture.h"

#include <SDL_image.h>
#include <stdexcept>

Texture::Texture(SDL_Renderer *renderer, string_view filename, bool useColorKey) : w{0}, h{0} {
    SDL_Surface *tempSurf = IMG_Load(filename.data());
    if (useColorKey) {
        SDL_SetColorKey(tempSurf, SDL_TRUE, SDL_MapRGB(tempSurf->format, 0xff, 0x00, 0xff));
    }
    texture = SDL_CreateTextureFromSurface(renderer, tempSurf);
    SDL_FreeSurface(tempSurf);
    if (!texture)
        throw std::runtime_error("Unable to create texture!");
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
}

Texture::Texture(SDL_Renderer *renderer, SDL_Surface *surf) : w{0}, h{0} {
    texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (!texture)
        throw std::runtime_error("Unable to create texture from surface!");
    SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
}

Texture::~Texture() {
    SDL_DestroyTexture(texture);
}

void Texture::render(SDL_Renderer *renderer, SDL_Rect &srcRect, SDL_Rect &dstRect) {
    SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
}

void Texture::render(SDL_Renderer *renderer, SDL_Rect &srcRect, int dstX, int dstY) {
    SDL_Rect dstRect = {dstX, dstY, srcRect.w, srcRect.h};
    render(renderer, srcRect, dstRect);
}

void Texture::render(SDL_Renderer *renderer, SDL_Rect &dstRect) {
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);
}

void Texture::render(SDL_Renderer *renderer, int x, int y) {
    SDL_Rect rect = {x, y, w, h};
    render(renderer, rect);
}
