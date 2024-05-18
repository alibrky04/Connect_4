// Main function for "Connect 4" project

#include <iostream>
#include <unistd.h> 
#include <tuple>
#include "GameController.h"

#define MAIN 0
#define VERSUS 1
#define PVP 2
#define PVA 3
#define GAMEOVER 4

int main(int argc, char* argv[])
{
    GameController gameController;

    std::tuple<bool, int> linkWithSDL(true, 1);

    gameController.SDLController.init();
    gameController.SDLController.loadMedia();

    while (std::get<0>(linkWithSDL)) {
        linkWithSDL = gameController.SDLController.handleMenuEvents();

        switch (std::get<1>(linkWithSDL)) {
        case MAIN:
            gameController.SDLController.renderMenu();
            break;

        case PVP:
            linkWithSDL = gameController.p2GameLoop();
            break;

        case PVA:
            linkWithSDL = gameController.aiGameLoop();
            break;

        default:
            std::cerr << "There is an error with the link to SDL controller!" << std::endl;
            break;
        }
    }

    gameController.SDLController.clean();

    return 0;
}
