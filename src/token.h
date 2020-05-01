#ifndef UNBORED_GAME_TOKEN_H
#define UNBORED_GAME_TOKEN_H

#include <vector>
#include <SDL.h>

#include "connections.h"
#include "texture.h"

using std::vector;

/*
 * Token has required attribute
 *  image (string) - resource location in assets directory
 * And optional attributes
 *  start (string) - Node name to start at
 *  id (string) - an ID to reference this token (TODO: not implemented yet)
 */
class Token {
    Path *currentPath;
    int pathTicks;
public:
    Node *currentNode = nullptr;
    Texture image;
    SDL_Rect rect{};
    bool hidden;

    Token(SDL_Renderer *renderer, const string &imageFilename, int x, int y, bool hidden);

    void setPath(Path *newPath);

    void setNode(Node *newNode);

    void selectOption(unsigned index);

    void tick();

    static void
    parse(pugi::xml_node doc_node, const string &dir_name, SDL_Renderer *renderer, vector<unique_ptr<Token>> &token_vec,
          vector<unique_ptr<Node>> &node_vec);
};

#endif //UNBORED_GAME_TOKEN_H
