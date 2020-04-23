#include "surface.h"

#include <stdexcept>
#include <SDL_image.h>

Surface::Surface(string_view filename) : w{0}, h{0} {
    surface = IMG_Load(filename.data());
    if (!surface)
        throw std::runtime_error("Unable to create surface!");
    SDL_Rect rect;
    SDL_GetClipRect(surface, &rect);
    w = rect.w;
    h = rect.h;
}

Surface::Surface(SDL_Surface *from) {
    surface = from;
    SDL_Rect rect;
    SDL_GetClipRect(surface, &rect);
    w = rect.w;
    h = rect.h;
}

Surface::~Surface() {
    delete surface;
}

void Surface::fill(const SDL_Rect *rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_FillRect(surface, rect, SDL_MapRGBA(surface->format, r, g, b, a));
}

void Surface::blit(Surface &src, SDL_Rect &rect) {
    SDL_BlitSurface(src.surface, nullptr, surface, &rect);
}

void Surface::blit(Surface &src, unsigned x, unsigned y) {
    SDL_Rect rect{static_cast<int>(x), static_cast<int>(y), w, h};
    blit(src, rect);
}
