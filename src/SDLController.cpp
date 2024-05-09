#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDLController.h"
#include "GameController.h"
#include <SDL2/SDL_image.h>

SDLController::SDLController()
{
    lastChosenColumn = 0;

    for (int i = 0; i < COLUMN; i++)
    {
        availableFirstSpotX[i] = gameBoardX + (columnWidth * i);
    }

    for (int i = 0; i < ROW; i++)
    {
        avalaibleFirstSpotY[i] = SCREEN_HEIGHT - gameBoardY - pieceSize;
    }
    
    pieceCounter = 0;
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
            int mouseY = e.button.y;

            if (mouseX > gameBoardX && mouseX < gameBoardX + gameBoardWidth)
            {
                if (mouseY > gameBoardY && mouseY < gameBoardY + gameBoardHeight)
                {
                    lastChosenColumn = (mouseX - gameBoardWidth / 4) / columnWidth + 1;

                    gamePieces[pieceCounter].x = availableFirstSpotX[lastChosenColumn - 1];
                    gamePieces[pieceCounter].y = avalaibleFirstSpotY[lastChosenColumn - 1];
                    gamePieces[pieceCounter].w = pieceSize;
                    gamePieces[pieceCounter].h = pieceSize;

                    avalaibleFirstSpotY[lastChosenColumn - 1] -= pieceSize;

                    pieceCounter++;

                    std::cout << "Player chose column: " << lastChosenColumn << std::endl;
                }
                
            }
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

bool SDLController::loadMedia()
{
    gameBoard = loadTexture("images/gameboard(empty).png");
    yellowPiece = loadTexture("images/connect4_tiles(yellow).png");
    redPiece = loadTexture("images/connect4_tiles(red).png");

    if (gameBoard == NULL) {
        std::cerr << "Failed to load game board texture!" << std::endl;
        return false;
    }
    else if (yellowPiece == NULL) {
        std::cerr << "Failed to load yellow piece texture!" << std::endl;
        return false;
    }
    else if (redPiece == NULL) {
        std::cerr << "Failed to load red piece texture!" << std::endl;
        return false;
    }
    return true;
}

void SDLController::renderGameBoard(int x, int y, int width, int height)
{
    SDL_Rect destRect = {x, y, width, height};
    SDL_RenderCopy(renderer, gameBoard, NULL, &destRect);
}

void SDLController::renderGameTiles()
{
    for (int i = 0; i < pieceCounter; i++)
    {
        if(i % 2 == 0) {SDL_RenderCopy(renderer, yellowPiece, NULL, &gamePieces[i]);}
        else {SDL_RenderCopy(renderer, redPiece, NULL, &gamePieces[i]);}
        
    }
}

void SDLController::render()
{
    SDL_RenderClear(renderer);

    renderGameBoard(gameBoardX, gameBoardY, gameBoardWidth, gameBoardHeight);

    renderGameTiles();

    SDL_SetRenderDrawColor(renderer, 173, 216, 230, SDL_ALPHA_OPAQUE);

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

    SDL_DestroyTexture(yellowPiece);
    yellowPiece = NULL;

    SDL_DestroyTexture(redPiece);
    redPiece = NULL;

    SDL_Quit();
}

int SDLController::getLastChosenColumn(){return lastChosenColumn;}

void SDLController::setLastChosenColumn(int newColumn){lastChosenColumn = newColumn;}

SDLController::~SDLController()
{

}
