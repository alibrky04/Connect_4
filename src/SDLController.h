#ifndef __SDLCONTROLLER__
#define __SDLCONTROLLER__

#include <iostream>
#include <string>
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>

class SDLController {
private:
    const int SCREEN_WIDTH = 560;
    const int SCREEN_HEIGHT = 480;

    int lastChosenColumn;

    SDL_Window *window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* gameBoard = NULL;
public:
    SDLController();

    bool init();

    bool handleEvents();

    SDL_Texture* loadTexture (std::string path);

    bool loadMedia();

    void render();

    void clean();

    int getLastChosenColumn();
    void setLastChosenColumn(int newColumn);
    
    ~SDLController();
};


#endif // !__SDLCONTROLLER__