// Main function for "Connect 4" project

#include <iostream>
#include "Game_Table.h"
#include "GameController.h"
#include "SDLController.h"

int main(int argc, char* argv[])
{
    GameController gameController;
    SDLController SDLController;

    bool quit = true;

    SDLController.init();
    SDLController.loadMedia();

    while (quit)
    {
        quit = SDLController.handleEvents();

        SDLController.render();
    }

    SDLController.clean();

    return 0;
}
