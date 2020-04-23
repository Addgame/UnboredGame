#ifndef UNBORED_GAME_APPLICATION_H
#define UNBORED_GAME_APPLICATION_H

#include <iostream>
#include <vector>
#include <chrono>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class Application;

#include "connections.h"
#include "screen.h"

using std::string_view;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 1024;

const float TIMESTEP = 1 / 60.0f;

class Application {
private:
    SDL_Window *window;

    IScreen *screen;

    bool quit;

public:
    Application();

    ~Application();

    static void log(void *userdata, int category, SDL_LogPriority priority, const char *message);

    IScreen *getScreen();

    void setScreen(IScreen *new_screen, bool delete_old = true);

    void setQuit();

    int run();

    SDL_Renderer *renderer;
};

#endif //UNBORED_GAME_APPLICATION_H
