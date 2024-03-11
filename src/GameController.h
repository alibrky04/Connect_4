#ifndef __GAMECONTROLLER__
#define __GAMECONTROLLER__

#include <vector>
#include "Game_Table.h"

class GameController {
private:
	
public:
	GameController(); // Constructor for the class

	void p2GameLoop(); // Starts game loop, ends when a player loses
    
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
