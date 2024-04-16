#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDLController.h"
#include "GameController.h"
#include <SDL2/SDL_image.h>

#define COLUMN 7

SDLController::SDLController()
{

}

bool SDLController::init()
{
    bool success = true;

    if(SDL_Init( SDL_INIT_VIDEO ) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        window = SDL_CreateWindow("Connect 4", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == NULL)
        {
            std::cerr << "Window could not be created! SDL_Error: " << IMG_GetError() << std::endl;
            success = false;
        }
        else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(renderer == NULL)
            {
                std::cerr << "Renderer could not be created! SDL_Error: " << IMG_GetError() << std::endl;
                success = false;
            }
        }
    }

    return success;
}

bool SDLController::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int column = mouseX / (SCREEN_WIDTH / COLUMN) + 1;

            std::cout << "Player chose column: " << column << std::endl;


        }
    }
    return true;
}

SDL_Texture* SDLController::loadTexture(std::string path)
{
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
    } else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL) {
            std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}

bool SDLController::loadMedia() {
    gameBoard = loadTexture("images/gameboard(empty).png");
    if (gameBoard == NULL) {
        std::cerr << "Failed to load game board texture!" << std::endl;
        return false;
    }
    return true;
}

void SDLController::render()
{
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, gameBoard, NULL, NULL);

    SDL_RenderPresent(renderer);
}

void SDLController::clean()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    SDL_DestroyTexture(gameBoard);
    gameBoard = NULL;

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

SDLController::~SDLController()
{

}
