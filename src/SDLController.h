#ifndef __SDLCONTROLLER__
#define __SDLCONTROLLER__

#include <iostream>
#include <string>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#define COLUMN 7
#define ROW 6

class SDLController {
private:
    const int SCREEN_WIDTH = 1120;
    const int SCREEN_HEIGHT = 960;
    const int gameBoardWidth = SCREEN_WIDTH / 1.5;
    const int gameBoardHeight = SCREEN_HEIGHT / 1.5;
    const int gameBoardX = (SCREEN_WIDTH - gameBoardWidth) / 2;
    const int gameBoardY = (SCREEN_HEIGHT - gameBoardHeight) / 2;
    const int columnWidth = gameBoardWidth / COLUMN;
    const int pieceSize = gameBoardHeight / ROW;

    int lastChosenColumn;
    int availableFirstSpotX[COLUMN];
    int avalaibleFirstSpotY[COLUMN];
    int pieceCounter;
    int mouseColumn;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* gameBoard = NULL;
    SDL_Texture* yellowPiece = NULL;
    SDL_Texture* redPiece = NULL;
    SDL_Texture* cursor = NULL;
    SDL_Rect gamePieces[42];

    TTF_Font* gameFont = NULL;
    SDL_Texture* textTexture = NULL;
public:
    SDLController();

    bool init();

    bool handleEvents();

    SDL_Texture* loadTexture(std::string path);

    bool loadMedia();

    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

    void renderGameBoard();

    void renderGameTiles();

    void renderCursor();

    void render();

    void clean();

    int getLastChosenColumn();

    void setLastChosenColumn(int newColumn);
    
    ~SDLController();
};


#endif // !__SDLCONTROLLER__