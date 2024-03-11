// Main function for "Connect 4" project

#include <iostream>
#include "Game_Table.h"
#include "GameController.h"
#include "SDLController.h"

int main(int argc, char* argv[])
{
    GameController gameController;
    SDLController SDLController;

    if (!SDLController.init()) {
        printf("Failed to initialize!\n");
    }
    else {
        if (!SDLController.handleEvents()) {
            printf( "Failed to load media!\n" );
        }
        else {
            SDLController.update();
        }
    }

    SDLController.clean();

    return 0;
}
