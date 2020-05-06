#include <ctime>
#include <cassert>

#include "application.h"

Application::Application() {
    window = nullptr;
    renderer = nullptr;
    screen = nullptr;
    quit = false;
}

Application::~Application() {
    delete screen;

    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;

    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Application::log(void *userdata, int category, SDL_LogPriority priority, const char *message) {
    time_t now;
    struct tm *ts;
    time(&now);
    ts = localtime(&now);
    printf("[%d-%02d-%02d %02d:%02d:%02d] %s", ts->tm_year + 1900, ts->tm_mon + 1, ts->tm_mday,
           ts->tm_hour, ts->tm_min, ts->tm_sec, message);
    string_view sdl_error{SDL_GetError()};
    if (!sdl_error.empty()) {
        std::cout << " SDL Error: " << sdl_error;
        SDL_ClearError();
    }
    std::cout << std::endl;
}

IScreen *Application::getScreen() {
    return screen;
}

// delete old defaults to true
void Application::setScreen(IScreen *new_screen, bool delete_old) {
    IScreen *old_screen = screen;

    if (old_screen)
        old_screen->hide();
    screen = new_screen;
    new_screen->show();

    if (delete_old) {
        delete old_screen;
    }
}

void Application::setQuit() {
    quit = true;
}

int Application::run() {
    SDL_LogSetOutputFunction(Application::log, nullptr);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_EVENTS)) {
        SDL_Log("Could not initialize SDL!");
        return 1;
    }

    IMG_InitFlags image_flags = IMG_INIT_PNG;
    if (!(IMG_Init(image_flags) & image_flags)) {
        SDL_Log("Could not initialize SDL_image!");
        return 1;
    }

    if (TTF_Init()) {
        SDL_Log("Could not initialize SDL_ttf!");
        return 1;
    }

    MIX_InitFlags mix_flags = MIX_INIT_OGG;
    if (!(Mix_Init(mix_flags) & mix_flags)) {
        SDL_Log("Could not initialize SDL_mixer file type loading!");
        return 1;
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 1, 2048)) {
        SDL_Log("Could not initialize SDL_mixer audio!");
        return 1;
    }

    window = SDL_CreateWindow("Unbored Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_SHOWN); // || SDL_WINDOW_FULLSCREEN);
    if (window == nullptr) {
        SDL_Log("Could not create window!");
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDL_Log("Could not create renderer!");
        return 1;
    }
    if (*SDL_GetError()) {
        SDL_Log("Non-fatal error.");
    }

    setScreen(new MenuScreen(*this));

    assert(screen != nullptr);

    using std::chrono::time_point;
    using std::chrono::steady_clock;
    using std::chrono::duration;
    using std::min;

    float accumulator = 0;
    time_point<steady_clock> prev, now;
    duration<float> raw_frame_time{};
    prev = steady_clock::now();
    SDL_Event event;
    while (!quit) {
        now = steady_clock::now();
        raw_frame_time = now - prev;
        prev = now;
        float frame_time = min(raw_frame_time.count(), 0.25f);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                setQuit();
                continue;
            }
            screen->handleEvent(event);
        }
        accumulator += frame_time;
        while (accumulator > TIMESTEP) {
            screen->update();
            accumulator -= TIMESTEP;
        }
        screen->render();
        SDL_Delay(10);
    }

    screen->hide();

    return 0;
}
