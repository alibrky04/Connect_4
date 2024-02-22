// Main function for "Connect 4" project

#include <iostream>
#include "Game_Table.h"
#include "functions.h"
#include <SDL2/SDL.h>

int main(int argc, char* argv[])
{
    
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_ALLOW_HIGHDPI);

    if(window == NULL){
        std::cout << "Error" << SDL_GetError()<< std::endl;
        return 1;
    }

    SDL_Event windowEvent;

    while(true){
        if(SDL_PollEvent(&windowEvent)){
            if(SDL_QUIT == windowEvent.type)
            break;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
    

    ai_game_loop();
}
