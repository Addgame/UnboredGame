#include <iostream>
#include <vector>
#include <map>
#include <cmath>

#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <pugixml/src/pugixml.hpp>

using std::string;

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 1024;

TTF_Font *font;

const SDL_Color BLACK = {0, 0, 0};
const SDL_Color WHITE = {255, 255, 255};

class Player {

};

class Game {
public:
    std::vector<Player> players;
};


class GameMeta {
public:
    string name;
    string version;
    string description;

    SDL_Surface *image;

    GameMeta(const string &name, const string &version, const string &description, const string &filename) {
        this->name = name;
        this->version = version;
        this->description = description;

        this->image = SDL_CreateRGBSurface(0, 200, 400, 32, 0, 0, 0, 0);
        SDL_FillRect(this->image, nullptr, SDL_MapRGB(this->image->format, 0, 0, 0));

        SDL_Surface *previewImage = SDL_LoadBMP(filename.c_str());

        SDL_BlitSurface(previewImage, nullptr, this->image, nullptr);

        SDL_Rect tempRect{};
        tempRect.x = 0;
        tempRect.y = 200;

        SDL_Surface *tempTextImage;

        tempTextImage = TTF_RenderText_Solid(font, name.c_str(), WHITE);
        SDL_BlitSurface(tempTextImage, nullptr, this->image, &tempRect);
        tempRect.y += tempTextImage->h;

        tempTextImage = TTF_RenderText_Solid(font, version.c_str(), WHITE);
        SDL_BlitSurface(tempTextImage, nullptr, this->image, &tempRect);
        tempRect.y += tempTextImage->h;

        tempTextImage = TTF_RenderText_Solid(font, description.c_str(), WHITE);
        SDL_BlitSurface(tempTextImage, nullptr, this->image, &tempRect);
    }
};

class Path;

class Node {
public:
    std::vector<Path *> outPaths;
    int x, y;
    string id;

    Node(const string &id, int x, int y) {
        this->id = id;
        this->x = x;
        this->y = y;
    }

    void addOutPath(Path *path) {
        outPaths.push_back(path);
    }
};

class Path {
public:
    int dx, dy, numTicks;

    Node *from;
    Node *to;

    Path(Node &from, Node &to) : Path(from, to, 10) {

    }

    Path(Node &from, Node &to, int speed) {
        this->from = &from;
        this->to = &to;
        double distance = sqrt(pow(to.x - from.x, 2) + pow(to.y - from.y, 2));
        numTicks = static_cast<int>(distance / speed);
        std::cout << "distance: " << distance;
        std::cout << " num ticks: " << numTicks;
        dx = (to.x - from.x) / numTicks;
        dy = (to.y - from.y) / numTicks;
        std::cout << " dx: " << dx;
        std::cout << " dy: " << dy << std::endl;
    }
};

class GameObject {
private:
    Node *currentNode = nullptr;
    Path *currentPath;
    int pathTicks;
public:
    SDL_Surface *image;
    SDL_Rect rect;

    GameObject(const string &imageFilename, int x, int y) {
        image = SDL_LoadBMP(imageFilename.c_str());
        currentPath = nullptr;
        SDL_GetClipRect(image, &rect);
        rect.x = x;
        rect.y = y;
        pathTicks = 0;
    }

    void setPath(Path *newPath) {
        currentPath = newPath;
        rect.x = currentPath->from->x;
        rect.y = currentPath->from->y;
        currentNode = currentPath->to;
        pathTicks = 0;
    }

    void setNode(Node *newNode) {
        currentNode = newNode;
        rect.x = newNode->x;
        rect.y = newNode->y;
    }

    void selectOption(int index) {
        std::cout << "in selecting" << std::endl;
        if (!currentNode) {
            return;
        }
        if ((index - 1) >= currentNode->outPaths.size()) {
            return;
        }

        std::cout << "before path" << std::endl;
        setPath(currentNode->outPaths[index - 1]);
        std::cout << "finished set path" << std::endl;
    }

    void tick() {
        if (currentPath) {
            pathTicks++;
            if (pathTicks >= currentPath->numTicks) {
                rect.x = currentPath->to->x;
                rect.y = currentPath->to->y;
                currentPath = nullptr;
            } else {
                rect.x += currentPath->dx;
                rect.y += currentPath->dy;
            }
        }
    }
};

enum AppState {
    PREVIEW,
    ACTIVE
};

int main() {

    AppState state = PREVIEW;

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

    GameObject mainObject{"../games/test game/assets/go.bmp", 0, 0};

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
}