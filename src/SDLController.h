#ifndef __SDLCONTROLLER__
#define __SDLCONTROLLER__

#include <iostream>
#include <string>
#include "GameController.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class SDLController {
private:
    const int SCREEN_WIDTH = 1120;
    const int SCREEN_HEIGHT = 960;

    bool isRunning;
    SDL_Window *window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* gameBoard = NULL;
public:
    GameController gameController;

    SDLController();

    bool init();
    bool handleEvents();
    SDL_Texture* loadTexture (std::string path);
    bool loadMedia();
    void render();
    void clean();
    
    ~SDLController();
};


#endif // !__SDLCONTROLLER__