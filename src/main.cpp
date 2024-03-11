// Main function for "Connect 4" project

#include <iostream>
#include "Game_Table.h"
#include "GameController.h"
#include "SDLController.h"

int main(int argc, char* argv[])
{
    GameController gameController;
    SDLController SDLController;

    gameController.p2GameLoop();

    return 0;
}
