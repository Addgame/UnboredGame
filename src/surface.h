#ifndef UNBORED_GAME_SURFACE_H
#define UNBORED_GAME_SURFACE_H

#include <string_view>
#include <SDL_surface.h>

using std::string_view;

class Surface {
public:
    SDL_Surface *surface;
    int w, h;

    explicit Surface(string_view);

    Surface(SDL_Surface *from);

    ~Surface();

    void fill(const SDL_Rect *rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    void blit(Surface &src, SDL_Rect &rect);

    void blit(Surface &src, unsigned x, unsigned y);
};

#endif //UNBORED_GAME_SURFACE_H
