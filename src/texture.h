#ifndef UNBORED_GAME_TEXTURE_H
#define UNBORED_GAME_TEXTURE_H

#include <string_view>
#include <SDL_render.h>

using std::string_view;

class Texture {
private:
    SDL_Texture *texture;
public:
    int w, h;

    Texture(SDL_Renderer *renderer, string_view filename, bool useColorKey = false);

    Texture(SDL_Renderer *renderer, SDL_Surface *surf);

    ~Texture();

    bool valid();

    void render(SDL_Renderer *renderer, SDL_Rect &rect);

    void render(SDL_Renderer *renderer, int x, int y);
};

#endif //UNBORED_GAME_TEXTURE_H
