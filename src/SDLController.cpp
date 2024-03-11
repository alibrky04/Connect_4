#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDLController.h"
#include "GameController.h"

SDLController::SDLController()
{
    
}

bool SDLController::init()
{
    bool success = true;

    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        window = SDL_CreateWindow( "Connect 4", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            success = false;
        }
        else { screenSurface = SDL_GetWindowSurface( window ); }
    }

    return success;
}

bool SDLController::handleEvents()
{
    bool success = true;

    return success;
}

SDL_Surface *SDLController::loadSurface(std::string path)
{
    SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );

    if( loadedSurface == NULL ) {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }

    return loadedSurface;
}

void SDLController::update()
{
    SDL_Event e; bool quit = false;

    while( !quit ) { 
        while( SDL_PollEvent( &e ) ) {
            if( e.type == SDL_QUIT ) { quit = true; }
        }

        SDL_BlitSurface( imageSurface, NULL, screenSurface, NULL );
        SDL_UpdateWindowSurface( window );
    }
}

void SDLController::render()
{
}

void SDLController::clean()
{
    SDL_FreeSurface(imageSurface);
    imageSurface = NULL;

    SDL_DestroyWindow( window );
    window = NULL;

    SDL_Quit();
}

bool SDLController::running()
{
    return false;
}

SDLController::~SDLController()
{

}
