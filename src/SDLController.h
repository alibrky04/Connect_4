#ifndef __SDLCONTROLLER__
#define __SDLCONTROLLER__

#include <iostream>
#include <string>
#include <SDL2/SDL.h>

class SDLController {
private:
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    enum KeyPressSurfaces {
        KEY_PRESS_SURFACE_DEFAULT,
        KEY_PRESS_SURFACE_UP,
        KEY_PRESS_SURFACE_DOWN,
        KEY_PRESS_SURFACE_LEFT,
        KEY_PRESS_SURFACE_RIGHT,
        KEY_PRESS_SURFACE_TOTAL
    };
    
    const std::string paths[KEY_PRESS_SURFACE_TOTAL] = {"images/press.bmp",
                                                        "images/up.bmp",
                                                        "images/down.bmp",
                                                        "images/left.bmp",
                                                        "images/right.bmp"};

    SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];

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