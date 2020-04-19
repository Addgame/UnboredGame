#include <iostream>

#include "token.h"

Token::Token(const string &imageFilename, int x, int y) {
    image = SDL_LoadBMP(imageFilename.c_str());
    currentPath = nullptr;
    SDL_GetClipRect(image, &rect);
    rect.x = x;
    rect.y = y;
    pathTicks = 0;
}

void Token::setPath(Path *newPath) {
    currentPath = newPath;
    rect.x = currentPath->from->x;
    rect.y = currentPath->from->y;
    currentNode = currentPath->to;
    pathTicks = 0;
}

void Token::setNode(Node *newNode) {
    currentNode = newNode;
    rect.x = newNode->x;
    rect.y = newNode->y;
}

void Token::selectOption(int index) {
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

void Token::tick() {
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
