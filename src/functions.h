#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include <vector>
#include "Game_Table.h"

void p2_game_loop(); // Starts game loop, ends when a player loses
int isGameEnded(const std::vector<std::vector<int>> Table, const int row, const int column, const int player); // Checks if the game has ended after a move
int columnQuestion(int player, const std::vector<std::vector<int>> Table); // Asks the player where they want to put their piece
bool isColumnFree(int column, const std::vector<std::vector<int>> Table); // Checks if the column player chose in columnQuestion() is free or not
std::vector <std::vector <int>> putPiece(std::vector<std::vector<int>> Table, int column, int* row, int player); // Puts the player's piece into the column they chose
int minimax(std::vector<std::vector<int>> Table, int depth, int player, int c, int r, int alpha, int beta);
void ai_game_loop();

#endif // !__FUNCTIONS__
