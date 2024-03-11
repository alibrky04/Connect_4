#ifndef __SDLCONTROLLER__
#define __SDLCONTROLLER__

#include <iostream>
#include <string>
#include <SDL2/SDL.h>

class SDLController {
private:
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    bool isRunning;
    SDL_Window *window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* screenSurface = NULL;
    SDL_Surface* imageSurface = NULL;
public:
    SDLController();

    bool init();
    bool handleEvents();
    SDL_Surface* loadSurface (std::string path);
    void update();
    void render();
    void clean();

    bool running();
    
    ~SDLController();
};


#endif // !__SDLCONTROLLER__