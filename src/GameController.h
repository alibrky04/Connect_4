#ifndef __GAMECONTROLLER__
#define __GAMECONTROLLER__

#include <vector>
#include "Game_Table.h"
#include "SDLController.h"

class GameController {
private:
    const int ROW = 6;
    const int COLUMN = 7;
    const int MOD = 4;
    const int HUMAN = 1;
    const int AI = 2;
    const int TIE = 0;
    const int CONT = 3;
    const int MAX_SPACE_TO_WIN = 3;
	
public:
    SDLController SDLController;

	GameController(); // Constructor for the class

	bool p2GameLoop(); // Starts game loop, ends when a player loses
    
    int isGameEnded(const std::vector<std::vector<int>> Table, const int row, const int column, const int player); // Checks if the game has ended after a move
    
    int columnQuestion(const int player, const std::vector<std::vector<int>> Table); // Asks the player where they want to put their piece
    
    bool isColumnFree(const int column, const std::vector<std::vector<int>> Table); // Checks if the column player chose in columnQuestion() is free or not
    
    std::vector <std::vector <int>> putPiece(std::vector<std::vector<int>> Table, const int column, int* row, const int player); // Puts the player's piece into the column they chose
    
    void aiGameLoop();
    
    int minimax(const std::vector<std::vector<int>> Table, int depth, const int player, const int c, int r, int alpha, int beta);
    
    int evaluateMove(const std::vector<std::vector<int>> Table, const int col, const int player);
    
    int evaluateAdjacents(const int* adjacentPieces, const int player);

	~GameController(); // Destructor for the class
};

#endif // !__GAMECONTROLLER__
