#ifndef __SDLCONTROLLER__
#define __SDLCONTROLLER__

#include <iostream>
#include <string>
#include <tuple>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#define COLUMN 7
#define ROW 6

#define X 0
#define Y 1
#define W 2
#define H 3

#define MAIN 0
#define VERSUS 1
#define PVP 2
#define PVA 3
#define GAMEOVER 4

class SDLController {
private:
    const int SCREEN_WIDTH = 1120;
    const int SCREEN_HEIGHT = 960;
    const int gameBoardAttributes[4] = {(SCREEN_WIDTH - int(SCREEN_WIDTH / 1.5)) / 2,
                                        (SCREEN_HEIGHT - int(SCREEN_HEIGHT / 1.5)) / 2, 
                                        int(SCREEN_WIDTH / 1.5), int(SCREEN_HEIGHT / 1.5)};

    const int columnWidth = gameBoardAttributes[W] / COLUMN;
    const int pieceSize = gameBoardAttributes[H] / ROW;

    const int playButtonAttributes[4] = {SCREEN_WIDTH / 2 - int(SCREEN_WIDTH / 4.67 / 2), SCREEN_HEIGHT / 2,
                                         int(SCREEN_WIDTH / 4.67), int(SCREEN_HEIGHT / 19.2)};

    const int exitButtonAttributes[4] = {SCREEN_WIDTH / 2 - int(SCREEN_WIDTH / 4.67 / 2), SCREEN_HEIGHT / 2 + 150,
                                         int(SCREEN_WIDTH / 4.67), int(SCREEN_HEIGHT / 19.2)};

    const int pvpButtonAttributes[4] = {SCREEN_WIDTH / 2 - int(SCREEN_WIDTH / 2 / 2), SCREEN_HEIGHT / 2,
                                        int(SCREEN_WIDTH / 2), int(SCREEN_HEIGHT / 19.2)};

    const int pvaButtonAttributes[4] = {SCREEN_WIDTH / 2 - int(SCREEN_WIDTH / 2 / 2), SCREEN_HEIGHT / 2 + 150,
                                        int(SCREEN_WIDTH / 2), int(SCREEN_HEIGHT / 19.2)};

    const int rematchButtonAttributes[4] = {SCREEN_WIDTH / 2 - int(SCREEN_WIDTH / 4.67 / 2), SCREEN_HEIGHT / 2,
                                            int(SCREEN_WIDTH / 4.67), int(SCREEN_HEIGHT / 19.2)};

    const int mainMenuButtonAttributes[4] = {SCREEN_WIDTH / 2 - int(SCREEN_WIDTH / 4.67 / 2), SCREEN_HEIGHT / 2 + 150,
                                             int(SCREEN_WIDTH / 4.67), int(SCREEN_HEIGHT / 19.2)};

    int lastChosenColumn;
    int availableFirstSpotX[COLUMN];
    int avalaibleFirstSpotY[COLUMN];
    int columnPieceNumber[COLUMN];
    int pieceCounter;
    int mouseColumn;
    int menuState;
    int gameMode;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* gameBoard = NULL;
    SDL_Texture* yellowPiece = NULL;
    SDL_Texture* redPiece = NULL;
    SDL_Texture* cursor = NULL;
    SDL_Texture* mainMenu = NULL;
    SDL_Rect gamePieces[42];

    TTF_Font* gameFont = NULL;
    SDL_Texture* playText = NULL;
    SDL_Texture* exitText = NULL;
    SDL_Texture* pvpText = NULL;
    SDL_Texture* pvaText = NULL;
    SDL_Texture* rematchText = NULL;
    SDL_Texture* mainMenuText = NULL;
public:
    SDLController();

    bool init();

    bool handleGameModeEvents();

    std::tuple<bool, int> handleMenuEvents();

    void pieceAdded(int chosenColumn);

    SDL_Texture* loadTexture(std::string path);

    SDL_Texture* loadFromRenderedText(std::string textureText, SDL_Color textColor);

    bool loadMedia();

    void renderGameBoard();

    void renderGameTiles();

    void renderCursor();

    void renderMenuButtons();

    void renderGameModes();

    void renderMenu();

    void resetAttributes();

    void clean();

    int getLastChosenColumn();

    void setLastChosenColumn(int newColumn);

    void setMenuState(int newState);

    void setGameMode(int newMode);

    ~SDLController();
};


#endif // !__SDLCONTROLLER__