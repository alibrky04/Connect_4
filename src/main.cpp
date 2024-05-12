// Main function for "Connect 4" project

#include <iostream>
#include <unistd.h> 
#include "Game_Table.h"
#include "GameController.h"
#include "SDLController.h"

int main(int argc, char* argv[])
{
    GameController gameController;

    bool quit = true;

    gameController.SDLController.init();
    gameController.SDLController.loadMedia();

    while (quit)
    {
        quit = gameController.p2GameLoop();
        sleep(3);
    }

    gameController.SDLController.clean();

    return 0;
}
