#include "application.h"


int main(int argc, char *argv[]) {
    Application app;
    return app.run();
}

/*
int old_main(int argc, char *argv[]) {

    pugi::xml_document doc;

    pugi::xml_parse_result result = doc.load_file("../games/test game/game.xml");


    SDL_Window *window;
    SDL_Surface *screen;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Error in SDL initialization" << std::endl;
        return 1;
    }


    window = SDL_CreateWindow("Unbored Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                              SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "Error creating window" << std::endl;
        return 2;
    }

    screen = SDL_GetWindowSurface(window);
    SDL_Rect *screen_rect = nullptr;
    SDL_GetClipRect(screen, screen_rect);

    if (TTF_Init() < 0) {
        std::cout << "Error in TTF initiliazation" << std::endl;
        return 3;
    }

    font = TTF_OpenFont("../assets/MuktaMahee-Regular.ttf", 16);

    if (!font) {
        std::cout << "Error opening font" << std::endl;
        return 15;
    }

    auto metaNode = doc.child("Game").child("Meta");
    GameMeta staticMeta{metaNode.attribute("name").value(),
                        metaNode.attribute("version").value(),
                        metaNode.attribute("description").value(),
                        metaNode.attribute("image").value()};

    Token mainObject{"../games/test game/assets/go.bmp", 0, 0};

    std::map<string, Node *> nodes;
    for (pugi::xml_node node = doc.child("Game").child("Nodes").child("Node"); node; node = node.next_sibling("Node")) {
        string id = node.attribute("id").value();
        Node *temp = new Node(id, node.attribute("x").as_int(), node.attribute("y").as_int());

        nodes.insert(std::pair<string, Node *>(id, temp));
    }

    if (!nodes.empty()) mainObject.setNode(nodes["start"]);

    std::vector<Path *> paths;
    for (pugi::xml_node path = doc.child("Game").child("Paths").child("Path"); path; path = path.next_sibling("Path")) {
        Node *from_node = nodes[path.attribute("from").value()];
        paths.push_back(new Path(*from_node, *nodes[path.attribute("to").value()]));
        from_node->addOutPath(paths.back());
    }

    SDL_Surface *background = SDL_LoadBMP("../games/test game/assets/bg.bmp");


    SDL_Rect metaRect;
    metaRect.x = 0;
    metaRect.y = 0;

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        if (state == PREVIEW) {
            SDL_FillRect(screen, nullptr, SDL_MapRGB(screen->format, 0x00, 0xFF, 0xFF));
            SDL_BlitSurface(staticMeta.image, nullptr, screen, &metaRect);
        } else {
            mainObject.tick();
            SDL_BlitSurface(background, nullptr, screen, nullptr);
            SDL_BlitSurface(mainObject.image, nullptr, screen, &mainObject.rect);
        }

        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                if (state == PREVIEW) {
                    switch (event.key.keysym.sym) {
                        case SDLK_UP:
                            metaRect.y -= 3;
                            break;
                        case SDLK_DOWN:
                            metaRect.y += 3;
                            break;
                        case SDLK_LEFT:
                            metaRect.x -= 3;
                            break;
                        case SDLK_RIGHT:
                            metaRect.x += 3;
                            break;
                        case SDLK_w:
                            state = ACTIVE;
                            break;
                        case SDLK_s:
                            state = PREVIEW;
                            break;
                    }
                } else {
                    switch (event.key.keysym.sym) {
                        case SDLK_1:
                            std::cout << "selecting 1" << std::endl;
                            mainObject.selectOption(1);
                            break;
                        case SDLK_2:
                            mainObject.selectOption(2);
                            break;
                        case SDLK_3:
                            mainObject.selectOption(3);
                            break;
                        case SDLK_4:
                            mainObject.selectOption(4);
                            break;
                        case SDLK_5:
                            mainObject.selectOption(5);
                            break;
                        case SDLK_w:
                            state = ACTIVE;
                            break;
                        case SDLK_s:
                            state = PREVIEW;
                            break;
                    }
                }
            }
        }

        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }

    SDL_DestroyWindow(window);

    return 0;
}*/
