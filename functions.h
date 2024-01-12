#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include <vector>
#include "Game_Table.h"
#define ROW 6
#define COLUMN 7

void p2_game_loop();
bool isGameEnded(const std::vector<std::vector<int>> Table, const int row, const int column, const int player);
int columnQuestion(int player, const std::vector<std::vector<int>> Table);
bool isColumnFree(int column, const std::vector<std::vector<int>> Table);
std::vector <std::vector <int>> putPiece(std::vector<std::vector<int>> Table, int column, int* row, int player);

#endif // !__FUNCTIONS__
