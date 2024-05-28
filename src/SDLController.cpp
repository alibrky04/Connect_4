#include <stdio.h>
#include "SDL2/SDL.h"
#include "SDLController.h"
#include "GameController.h"
#include <SDL2/SDL_image.h>

SDLController::SDLController()
{
    lastChosenColumn = 0;
    pieceCounter = 0;
    mouseColumn = 0;
    menuState = 0;
    gameMode = 2;
    whichButton = -1;

    for (int i = 0; i < COLUMN; i++)
    {
        availableFirstSpotX[i] = gameBoardAttributes[X] + (columnWidth * i);
        avalaibleFirstSpotY[i] = SCREEN_HEIGHT - gameBoardAttributes[Y] - pieceSize;
        columnPieceNumber[i] = 0;
    }
}

bool SDLController::init()
{
    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
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
                    std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
                    success = false;
                }

                if(TTF_Init() == -1)
                {
                    std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
                    success = false;
                }

                if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                {
                    std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
                    success = false;
                }
            }
        }
    }

    return success;
}

bool SDLController::handleGameModeEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;

            if (handleMouseEvent(mouseX, mouseY, gameBoardAttributes)){
                lastChosenColumn = (mouseX - gameBoardAttributes[W] / 4) / columnWidth + 1;

                if (columnPieceNumber[lastChosenColumn - 1] < ROW)
                {
                    pieceAdded(lastChosenColumn);
                    Mix_PlayChannel(-1, dropSound, 0);
                    std::cout << "Player chose column: " << lastChosenColumn << std::endl;
                }
            }
        }

        int mouseX, mouseY;
        mouseColumn = -1;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
        if(mouseY > gameBoardAttributes[Y] && mouseY < gameBoardAttributes[Y] + gameBoardAttributes[H]){
            for (int i = 0; i < COLUMN; i++) {
                if (mouseX > gameBoardAttributes[X] && mouseX < gameBoardAttributes[X] + (i + 1) * columnWidth) {
                    mouseColumn = i;
                    break;
                }
            }
        }       
    }
    return true;
}

std::tuple<bool, int> SDLController::handleMenuEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return std::make_tuple(false, MAIN);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            int mouseX = e.button.x;
            int mouseY = e.button.y;
            
            switch (menuState) {
            case MAIN:
                if (handleMouseEvent(mouseX, mouseY, playButtonAttributes)){
                    menuState = VERSUS;
                    return std::make_tuple(true, MAIN);
                }
                if (handleMouseEvent(mouseX, mouseY, exitButtonAttributes)){
                    return std::make_tuple(false, MAIN);
                }
                break;

            case VERSUS:
                if (handleMouseEvent(mouseX, mouseY, pvpButtonAttributes)){
                    menuState = PVP;
                    return std::make_tuple(true, PVP);
                }
                if (handleMouseEvent(mouseX, mouseY, pvaButtonAttributes)){
                    menuState = PVA;
                    return std::make_tuple(true, PVA);
                }
                break;

            case GAMEOVER:
                if (handleMouseEvent(mouseX, mouseY, rematchButtonAttributes)){
                    if (!Mix_Playing(1)) {
                        resetAttributes();
                        return std::make_tuple(true, gameMode);
                    }
                }
                if (handleMouseEvent(mouseX, mouseY, mainMenuButtonAttributes)){
                    if (!Mix_Playing(1)) {
                        menuState = MAIN;
                        resetAttributes();
                        return std::make_tuple(true, MAIN);
                    }
                }
                break;
            
            default:
                std::cerr << "There is an error with the window state!" << std::endl;
                break;
            }
        }

        int mouseX, mouseY;
        mouseColumn = -1;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

        switch (menuState) {
            case MAIN:
                if (handleMouseEvent(mouseX, mouseY, playButtonAttributes)) {whichButton = 0;}
                else if (handleMouseEvent(mouseX, mouseY, exitButtonAttributes)) {whichButton = 1;}
                else {whichButton = -1;}
                break;

            case VERSUS:
                if (handleMouseEvent(mouseX, mouseY, pvpButtonAttributes)) {whichButton = 2;}
                else if (handleMouseEvent(mouseX, mouseY, pvaButtonAttributes)) {whichButton = 3;}
                else {whichButton = -1;}
                break;

            case GAMEOVER:
                if (handleMouseEvent(mouseX, mouseY, rematchButtonAttributes)) {whichButton = 4;}
                else if (handleMouseEvent(mouseX, mouseY, mainMenuButtonAttributes)) {whichButton = 5;}
                else {whichButton = -1;}
                break;
            
            default:
                whichButton = -1;
                std::cerr << "There is an error with the window state!" << std::endl;
                break;
        }
    }
    return std::make_tuple(true, MAIN);
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
    bool success = true;

    gameBoard = loadTexture("images/gameboard(empty).png");
    yellowPiece = loadTexture("images/connect4_tiles(yellow).png");
    redPiece = loadTexture("images/connect4_tiles(red).png");
    cursor = loadTexture("images/cursor.png");
    cursorRotated = loadTexture("images/cursorRotated.png");
    mainMenu = loadTexture("images/main_menu.png");

    if (gameBoard == NULL) {
        std::cerr << "Failed to load game board texture!" << std::endl;
        success = false;
    }
    else if (yellowPiece == NULL) {
        std::cerr << "Failed to load yellow piece texture!" << std::endl;
        success = false;
    }
    else if (redPiece == NULL) {
        std::cerr << "Failed to load red piece texture!" << std::endl;
        success = false;
    }
    else if (cursor == NULL) {
        std::cerr << "Failed to load cursor texture!" << std::endl;
        success = false;
    }
    else if (mainMenu == NULL) {
        std::cerr << "Failed to load main menu texture!" << std::endl;
        success = false;
    }
    else if (cursorRotated == NULL)
    {
        std::cerr << "Failed to load cursor rotated texture!" << std::endl;
        success = false;
    }
    

    gameFont = TTF_OpenFont("fonts/PressStart2P-Regular.ttf", 28);

    if (gameFont == NULL) {
        std::cerr << "Failed to load lazy font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        success = false;
    }
    else {
        SDL_Color textColor = { 0, 0, 0 };
        playText = loadFromRenderedText("Play", textColor);
        exitText = loadFromRenderedText("Exit", textColor);
        pvpText = loadFromRenderedText("Player vs. Player", textColor);
        pvaText = loadFromRenderedText("Player vs. AI Bot", textColor);
        rematchText = loadFromRenderedText("Rematch", textColor);
        mainMenuText = loadFromRenderedText("Main Menu", textColor);

        if (playText == NULL) {
            std::cerr << "Failed to render play texture!" << std::endl;
            success = false;
        }
        else if (exitText == NULL) {
            std::cerr << "Failed to render exit texture!" << std::endl;
            success = false;
        }
        else if (pvpText == NULL) {
            std::cerr << "Failed to render pvpText texture!" << std::endl;
            success = false;
        }
        else if (pvaText == NULL)
        {
            std::cerr << "Failed to render pvaText texture!" << std::endl;
            success = false;
        }
        else if (rematchText == NULL) {
            std::cerr << "Failed to render rematchText texture!" << std::endl;
            success = false;
        }
        else if (mainMenuText == NULL)
        {
            std::cerr << "Failed to render goToMainMenuText texture!" << std::endl;
            success = false;
        }
    }

    gameMusic = Mix_LoadMUS("sounds/music.wav");
    if(gameMusic == NULL)
    {
        printf( "Failed to load game music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    dropSound = Mix_LoadWAV("sounds/dropSound.wav");
    if(dropSound == NULL)
    {
        printf("Failed to load drop sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    
    winSound = Mix_LoadWAV("sounds/winSound.wav");
    if(winSound == NULL)
    {
        printf("Failed to load win sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    loseSound = Mix_LoadWAV("sounds/loseSound.wav");
    if(loseSound == NULL)
    {
        printf("Failed to load lose sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    tieSound = Mix_LoadWAV("sounds/tieSound.wav");
    if(tieSound == NULL)
    {
        printf("Failed to load tie sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    return success;
}

SDL_Texture* SDLController::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
    SDL_Texture* newTexture = NULL;

    SDL_Surface* textSurface = TTF_RenderText_Solid(gameFont, textureText.c_str(), textColor);
    if(textSurface == NULL) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;

    }
    else {
        newTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if(newTexture == NULL) {
            std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
        }

        SDL_FreeSurface(textSurface);
    }
    
    return newTexture;
}

void SDLController::renderGameBoard()
{
    SDL_Rect destRect = {gameBoardAttributes[X], gameBoardAttributes[Y], gameBoardAttributes[W], gameBoardAttributes[H]};
    SDL_RenderCopy(renderer, gameBoard, NULL, &destRect);
}

void SDLController::renderGameTiles()
{
    for (int i = 0; i < pieceCounter; i++) {
        if(i % 2 == 0) {SDL_RenderCopy(renderer, yellowPiece, NULL, &gamePieces[i]);}
        else {SDL_RenderCopy(renderer, redPiece, NULL, &gamePieces[i]);}
    }
}

void SDLController::renderCursor()
{
    if(mouseColumn != -1){
        const int cursorX = availableFirstSpotX[mouseColumn] + columnWidth / 4;
        const int cursorY = gameBoardAttributes[Y] - (pieceSize / 1.5);
        const int cursorWidth = pieceSize / 2;
        const int cursorHeight = pieceSize / 2;

        SDL_Rect cursorRect = {cursorX, cursorY, cursorWidth, cursorHeight};
        SDL_RenderCopy(renderer, cursor, NULL, &cursorRect);
    }
}

void SDLController::renderMenuButtons()
{
    SDL_Rect playButton, exitButton, pvpButton, pvaButton, rematchButton, mainMenuButton, whoWonTextRect, cursorRotatedRect;
    SDL_Color textColor = { 0, 0, 0 };

    int cursorX, cursorY;
    const int cursorWidth = pieceSize / 2;
    const int cursorHeight = pieceSize / 2;
    
    SDL_Rect rematchButtonFillRect, mainMenuButtonFillRect;

    switch (menuState) {
    case MAIN:
        playButton =    {playButtonAttributes[X], playButtonAttributes[Y],
                        playButtonAttributes[W], playButtonAttributes[H]};

        exitButton =    {exitButtonAttributes[X], exitButtonAttributes[Y],
                        exitButtonAttributes[W], exitButtonAttributes[H]};

        if (whichButton == 0) {
            cursorX = playButtonAttributes[X] - cursorWidth;
            cursorY = playButtonAttributes[Y] + int(playButtonAttributes[H] / 32);
        }
        else if (whichButton == 1)
        {
            cursorX = exitButtonAttributes[X] - cursorWidth;
            cursorY = exitButtonAttributes[Y] + int(exitButtonAttributes[H] / 32);
        }

        cursorRotatedRect = {cursorX, cursorY, cursorWidth, cursorHeight};

        if (whichButton != -1){SDL_RenderCopy(renderer, cursorRotated, NULL, &cursorRotatedRect);}
        

        SDL_RenderCopy(renderer, playText, NULL, &playButton);
        SDL_RenderCopy(renderer, exitText, NULL, &exitButton);
        break;
    
    case VERSUS:
        pvpButton = {pvpButtonAttributes[X], pvpButtonAttributes[Y],
                    pvpButtonAttributes[W], pvpButtonAttributes[H]};

        pvaButton = {pvaButtonAttributes[X], pvaButtonAttributes[Y],
                    pvaButtonAttributes[W], pvaButtonAttributes[H]};

        if (whichButton == 2) {
            cursorX = pvpButtonAttributes[X] - cursorWidth;
            cursorY = pvpButtonAttributes[Y] + int(pvpButtonAttributes[H] / 32);
        }
        else if (whichButton == 3)
        {
            cursorX = pvaButtonAttributes[X] - cursorWidth;
            cursorY = pvaButtonAttributes[Y] + int(pvaButtonAttributes[H] / 32);
        }

        cursorRotatedRect = {cursorX, cursorY, cursorWidth, cursorHeight};

        if (whichButton != -1){SDL_RenderCopy(renderer, cursorRotated, NULL, &cursorRotatedRect);}

        SDL_RenderCopy(renderer, pvpText, NULL, &pvpButton);
        SDL_RenderCopy(renderer, pvaText, NULL, &pvaButton);
        break;
    
    case GAMEOVER:
        switch (gameMode) {
        case PVP:
            if (pieceCounter == COLUMN * ROW) {whoWonText = loadFromRenderedText("It's a Tie!", textColor);}
            else if (pieceCounter % 2 != 0) {whoWonText = loadFromRenderedText("Player 1 Won!", textColor);}
            else {whoWonText = loadFromRenderedText("Player 2 Won!", textColor);}
            break;

        case PVA:
            if (pieceCounter == COLUMN * ROW) {whoWonText = loadFromRenderedText("It's a Tie!", textColor);}
            else if (pieceCounter % 2 != 0){whoWonText = loadFromRenderedText("AI Won!", textColor);}
            else {whoWonText = loadFromRenderedText("Player Won!", textColor);}
            break;
        
        default:
            std::cerr << "System Error!" << std::endl;
            break;
        }

        rematchButton = {rematchButtonAttributes[X], rematchButtonAttributes[Y],
                         rematchButtonAttributes[W], rematchButtonAttributes[H]};

        mainMenuButton = {mainMenuButtonAttributes[X], mainMenuButtonAttributes[Y],
                          mainMenuButtonAttributes[W], mainMenuButtonAttributes[H]};

        whoWonTextRect = {whoWonTextAttributes[X], whoWonTextAttributes[Y],
                          whoWonTextAttributes[W], whoWonTextAttributes[H]};
        
        rematchButtonFillRect = {rematchButtonAttributes[X] - int(rematchButtonAttributes[H] / 2),
                                 rematchButtonAttributes[Y] - int(rematchButtonAttributes[H] / 2),
                                 rematchButtonAttributes[W] + rematchButtonAttributes[H],
                                 rematchButtonAttributes[H] + rematchButtonAttributes[H]};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(renderer, &rematchButtonFillRect);

        mainMenuButtonFillRect = {mainMenuButtonAttributes[X] - int(mainMenuButtonAttributes[H] / 2),
                                  mainMenuButtonAttributes[Y] - int(mainMenuButtonAttributes[H] / 2),
                                  mainMenuButtonAttributes[W] + mainMenuButtonAttributes[H],
                                  mainMenuButtonAttributes[H] + mainMenuButtonAttributes[H]};
        SDL_RenderFillRect(renderer, &mainMenuButtonFillRect);

        if (whichButton == 4) {
            cursorX = rematchButtonAttributes[X] - int((rematchButtonFillRect.w - rematchButtonAttributes[W]) / 2) - cursorWidth;
            cursorY = rematchButtonAttributes[Y] + int(rematchButtonAttributes[H] / 32);
        }
        else if (whichButton == 5)
        {
            cursorX = mainMenuButtonAttributes[X] - int((mainMenuButtonFillRect.w - mainMenuButtonAttributes[W]) / 2) - cursorWidth;
            cursorY = mainMenuButtonAttributes[Y] + int(mainMenuButtonAttributes[H] / 32);
        }

        cursorRotatedRect = {cursorX, cursorY, cursorWidth, cursorHeight};

        if (whichButton != -1 && !Mix_Playing(1)){SDL_RenderCopy(renderer, cursorRotated, NULL, &cursorRotatedRect);}

        SDL_RenderCopy(renderer, rematchText, NULL, &rematchButton);
        SDL_RenderCopy(renderer, mainMenuText, NULL, &mainMenuButton);
        SDL_RenderCopy(renderer, whoWonText, NULL, &whoWonTextRect);
        break;
    
    default:
        std::cerr << "Main menu buttons couldn't render!" << std::endl;
        break;
    }
}

void SDLController::renderGameModes()
{
    SDL_RenderClear(renderer);

    renderGameBoard();
    renderGameTiles();
    renderCursor();

    SDL_SetRenderDrawColor(renderer, 173, 216, 230, SDL_ALPHA_OPAQUE);
    SDL_RenderPresent(renderer);
}

void SDLController::renderMenu()
{
    SDL_RenderClear(renderer);

    if (menuState == GAMEOVER) {
        renderGameBoard();
        renderGameTiles();
        renderMenuButtons();
    }
    else {
        SDL_RenderCopy(renderer, mainMenu, NULL, NULL);

        renderMenuButtons();
    }

    SDL_SetRenderDrawColor(renderer, 173, 216, 230, SDL_ALPHA_OPAQUE);
    SDL_RenderPresent(renderer);
}

void SDLController::resetAttributes()
{
    lastChosenColumn = 0;
    pieceCounter = 0;
    mouseColumn = 0;

    for (int i = 0; i < COLUMN; i++)
    {
        availableFirstSpotX[i] = gameBoardAttributes[X] + (columnWidth * i);
        avalaibleFirstSpotY[i] = SCREEN_HEIGHT - gameBoardAttributes[Y] - pieceSize;
        columnPieceNumber[i] = 0;
    }
}

void SDLController::playGameMusic()
{
    if (Mix_PlayingMusic() == 0 && menuState != GAMEOVER)
    {
        Mix_PlayMusic(gameMusic, -1);
        Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    }
}

void SDLController::stopGameMusic()
{
    if (Mix_PlayingMusic() == 1) {Mix_HaltMusic();}
}

void SDLController::playGameOverSound()
{
    if (Mix_PlayingMusic() == 1) {Mix_HaltMusic();}

    switch (gameMode) {
    case PVP:
        if (pieceCounter < COLUMN * ROW) {Mix_PlayChannel(1, winSound, 0);}
        else {Mix_PlayChannel(-1, tieSound, 0);}
        break;

    case PVA:
        if (pieceCounter == COLUMN * ROW) {Mix_PlayChannel(1, tieSound, 0);}
        else if (pieceCounter % 2 != 0) {Mix_PlayChannel(1, loseSound, 0);}
        else {Mix_PlayChannel(1, winSound, 0);}
        break;
    
    default:
        std::cerr << "System Error!" << std::endl;
        break;
    }
}

bool SDLController::handleMouseEvent(int mouseX, int mouseY, const int* buttonRectAttributes)
{
    SDL_Rect buttonRect = {buttonRectAttributes[X], buttonRectAttributes[Y],
                           buttonRectAttributes[W], buttonRectAttributes[H]};

    if (mouseX >= buttonRect.x && mouseX <= buttonRect.x + buttonRect.w &&
        mouseY >= buttonRect.y && mouseY <= buttonRect.y + buttonRect.h) {
        return true;
    } else {return false;}
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

    SDL_DestroyTexture(cursorRotated);
    cursorRotated = NULL;

    SDL_DestroyTexture(mainMenu);
    mainMenu = NULL;

    TTF_CloseFont(gameFont);
    gameFont = NULL;

    SDL_DestroyTexture(playText);
    playText = NULL;

    SDL_DestroyTexture(exitText);
    exitText = NULL;

    SDL_DestroyTexture(pvpText);
    pvpText = NULL;

    SDL_DestroyTexture(pvaText);
    pvaText = NULL;

    SDL_DestroyTexture(rematchText);
    rematchText = NULL;

    SDL_DestroyTexture(mainMenuText);
    mainMenuText = NULL;

    SDL_DestroyTexture(whoWonText);
    whoWonText = NULL;

    Mix_Music* gameMusic = NULL;

    Mix_FreeChunk(dropSound);
    Mix_FreeChunk(winSound);
    Mix_FreeChunk(loseSound);
    Mix_FreeChunk(tieSound);
    dropSound = NULL;
    winSound = NULL;
    loseSound = NULL;
    tieSound = NULL;

    Mix_FreeMusic(gameMusic);
    gameMusic = NULL;

    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int SDLController::getLastChosenColumn() {return lastChosenColumn;}

void SDLController::setLastChosenColumn(int newColumn) {lastChosenColumn = newColumn;}

void SDLController::setMenuState(int newState) {menuState = newState;}

void SDLController::setGameMode(int newMode) {gameMode = newMode;}

SDLController::~SDLController()
{

}
