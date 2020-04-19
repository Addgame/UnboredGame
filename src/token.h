#ifndef UNBORED_GAME_TOKEN_H
#define UNBORED_GAME_TOKEN_H

#include <vector>
#include <SDL.h>

#include "connections.h"

using std::vector;

class Token {
private:
    Node *currentNode = nullptr;
    Path *currentPath;
    int pathTicks;
public:
    SDL_Surface *image;
    SDL_Rect rect;

    Token(const string &imageFilename, int x, int y);

    void setPath(Path *newPath);

    void setNode(Node *newNode);

    void selectOption(int index);

    void tick();
};

class TokenOwner {
protected:
    vector<Token *> tokens;
};

#endif //UNBORED_GAME_TOKEN_H
