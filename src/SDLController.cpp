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
        avalaibleFirstSpotY[i] = SCREEN_HEIGHT - gameBoardY - pieceSize;
        columnPieceNumber[i] = 0;
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
            else
            {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if(!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }

                if(TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
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

                    if (columnPieceNumber[lastChosenColumn - 1] < ROW)
                    {
                        pieceAdded(lastChosenColumn);
                        std::cout << "Player chose column: " << lastChosenColumn << std::endl;
                    }
                }
            }
        }

        int mouseX, mouseY;
        mouseColumn = -1;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
        if(mouseY > gameBoardY && mouseY < gameBoardY + gameBoardHeight){
            if (mouseX > gameBoardX && mouseX < gameBoardX + columnWidth) {mouseColumn = 0;}
            else {
                for (int i = 1; i < COLUMN; i++) {
                    if (mouseX > gameBoardX && mouseX < gameBoardX + (i+1) * columnWidth) {
                        mouseColumn = i;
                        break;
                    }
                }
            }
        }       
    }
    return true;
}

void SDLController::pieceAdded(int chosenColumn)
{
    gamePieces[pieceCounter].x = availableFirstSpotX[chosenColumn - 1];
    gamePieces[pieceCounter].y = avalaibleFirstSpotY[chosenColumn - 1];
    gamePieces[pieceCounter].w = pieceSize;
    gamePieces[pieceCounter].h = pieceSize;

    avalaibleFirstSpotY[chosenColumn - 1] -= pieceSize;
    columnPieceNumber[chosenColumn - 1]++;

    pieceCounter++;
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
    cursor = loadTexture("images/cursor.png");

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
    else if (cursor == NULL) {
        std::cerr << "Failed to load cursor texture!" << std::endl;
        return false;
    }

    gameFont = TTF_OpenFont("fonts/PressStart2P-Regular.ttf", 28);

    if(gameFont == NULL)
    {
        std::cerr << "Failed to load lazy font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }
    else
    {
        SDL_Color textColor = { 0, 0, 0 };
        if( !loadFromRenderedText("Connect 4", textColor))
        {
            std::cerr << "Failed to render text texture!" << std::endl;
            return false;
        }
    }

    return true;
}

bool SDLController::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(gameFont, textureText.c_str(), textColor);
    if(textSurface == NULL) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
    else {
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if(textTexture == NULL) {
            std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
        }

        SDL_FreeSurface(textSurface);
    }
    
    return textTexture != NULL;
}

void SDLController::renderGameBoard()
{
    SDL_Rect destRect = {gameBoardX, gameBoardY, gameBoardWidth, gameBoardHeight};
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

void SDLController::renderCursor()
{
    if(mouseColumn != -1){
        int cursorX = availableFirstSpotX[mouseColumn] + columnWidth / 4;
        int cursorY = gameBoardY - (pieceSize / 1.5);
        int cursorWidth = pieceSize / 2;
        int cursorHeight = pieceSize / 2;

        SDL_Rect cursorRect = {cursorX, cursorY, cursorWidth, cursorHeight};
        SDL_RenderCopy(renderer, cursor, NULL, &cursorRect);
    }
}

void SDLController::render()
{
    SDL_RenderClear(renderer);

    renderGameBoard();

    renderGameTiles();

    renderCursor();

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

    SDL_DestroyTexture(cursor);
    cursor = NULL;

    SDL_DestroyTexture(textTexture);
    textTexture = NULL;

    TTF_CloseFont(gameFont);
    gameFont = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int SDLController::getLastChosenColumn() {return lastChosenColumn;}

void SDLController::setLastChosenColumn(int newColumn) {lastChosenColumn = newColumn;}

SDLController::~SDLController()
{

}
