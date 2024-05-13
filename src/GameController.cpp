#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "Game_Table.h"
#include "GameController.h"
#include "SDLController.h"

using namespace std;

GameController::GameController()
{

}

bool GameController::p2GameLoop()
{
    Game_Table game_table;

	bool quit = true;

	int gamemode = 1;

	int player = 1, chosen_column = 1, row = 0, result = CONT;

	do {
		SDLController.render();

		if (SDLController.getLastChosenColumn() != 0) {
			chosen_column = SDLController.getLastChosenColumn(); // Gets the column from the player

			if (!isColumnFree(chosen_column, game_table.getTable()))
			{
				SDLController.setLastChosenColumn(0);

				continue;
			}

			game_table.setTable(putPiece(game_table.getTable(), chosen_column, &row, player)); // Changes the table according to the chosen column
			
			game_table.printTable(gamemode);
			cout << endl;

			SDLController.setLastChosenColumn(0);

			result = isGameEnded(game_table.getTable(), row, chosen_column, player);

			if (player == 1)
				player = 2; // Changes player turn to the second player
			else
				player = 1; // Changes player turn to the first player
		}

		quit = SDLController.handleEvents();
	} while (result == CONT && quit); // Checks if the game has ended

	game_table.printTable(gamemode);

	cout << endl;

	if(result == TIE){
		cout << "it's a tie!" << endl;
	}
	else{
		cout << "Player " << result << " won!" << endl;
	}

	return false;
}

// Main solver function for solving if the game has ended or not
// Controls for directions of the last put piece
// Goes to the end of a direction to control all possibilities
int GameController::isGameEnded(const std::vector<std::vector<int>> Table, const int row, const int column, const int player)
{
    int counter = 0, new_row = row, new_column = column; // counter is for counting the number of pieces, new_row and new_column are for going to the end of a direction
	const int control_array_row[4] = { 0, -1, -1, -1 }, control_array_column[4] = { -1, 0, -1, 1 }; // row, column, 135 degrees, 45 degrees

	for (int i = 0; i < MOD; i++) {
		// This loop checks if another piece of the player is present in the left (right for 45 degrees) of the piece, if it is changes the index of new_row and new_column
		for (int k = 0; Table[row + control_array_row[i] + k * control_array_row[i]][column + control_array_column[i] + k * control_array_column[i]] == player; k++) {
			new_row = row + control_array_row[i] + k * control_array_row[i];
			new_column = column + control_array_column[i] + k * control_array_column[i];
		}

		// This loop counts to four if the player's piece is present for any direction, if not resets the counter
		for (int k = 0; k < MOD; k++) {
			if (Table[new_row - k * control_array_row[i]][new_column - k * control_array_column[i]] == player) {
				counter++;
			}
			else {
				break;
			}

			if (counter == MOD) {
				return player;
			}
		}

		counter = 0;
		new_row = row, new_column = column;
	}

	for (int k = 1; k <= COLUMN; k++) {
		if (isColumnFree(k, Table)) {
			return CONT;
		}
		else if (k + 1 == 8) {
			return TIE;
		}
	}

	return CONT;
}

int GameController::columnQuestion(const int player, const std::vector<std::vector<int>> Table)
{
    int column_choice;

	do {
		cout << "Where would you want to drop your piece? You are player " << player << endl;
		cout << "Your Choice: ";
		cin >> column_choice;
		if (!isColumnFree(column_choice, Table)) {
			cout << endl << column_choice << ". column is full!" << endl << endl;
		}
	} while (!isColumnFree(column_choice, Table));

	return column_choice;
}

bool GameController::isColumnFree(const int column, const std::vector<std::vector<int>> Table)
{
    for (int i = 1; i <= ROW; i++) {
		if (Table[i][column] == 0) {
			return true;
		}
	}
	return false;
}

std::vector<std::vector<int>> GameController::putPiece(std::vector<std::vector<int>> Table, const int column, int *row, const int player)
{
    for (int i = ROW; i > 0; i--) {
		if (Table[i][column] == 0) {
			*row = i; // changes the last put piece's row number for the isGameEnded() function
			Table[i][column] = player;
			break;
		}
	}

	return Table;
}

bool GameController::aiGameLoop()
{
    srand(time(NULL));

	Game_Table game_table;

	bool quit = true;

	int gamemode = 2;

	int player = AI, chosen_column, row = 0, result;

	do {
		SDLController.render();

		if (SDLController.getLastChosenColumn() != 0 && player == HUMAN)
		{
			chosen_column = SDLController.getLastChosenColumn(); // Gets the column from the player

			if (!isColumnFree(chosen_column, game_table.getTable()))
			{
				SDLController.setLastChosenColumn(0);

				continue;
			}

			game_table.setTable(putPiece(game_table.getTable(), chosen_column, &row, player)); // Changes the table according to the chosen column
			
			game_table.printTable(gamemode);
			cout << endl;

			SDLController.setLastChosenColumn(0);

			result = isGameEnded(game_table.getTable(), row, chosen_column, player);

			player = AI;
		}
		else if (player == AI)
		{
			int bestScore = INT_MIN, bestMove, score;

			for(chosen_column = 1; chosen_column < 8; chosen_column++){
				if(isColumnFree(chosen_column, game_table.getTable())){
					vector<vector<int>> newTable = putPiece(game_table.getTable(), chosen_column, &row, player);
					score = minimax(newTable, 8, player, chosen_column, row, INT_MIN, INT_MAX);

					if(score > bestScore){
						bestScore = score;
						bestMove = chosen_column;
					}
				}
			}

			chosen_column = bestMove;
			game_table.setTable(putPiece(game_table.getTable(), chosen_column, &row, player));
			SDLController.pieceAdded(chosen_column);
			result = isGameEnded(game_table.getTable(), row, chosen_column, player);

			player = HUMAN;

			game_table.printTable(gamemode);
			cout << endl;
		}

		quit = SDLController.handleEvents();
	} while (result == CONT && quit); // Checks if the game has ended

	game_table.printTable(gamemode);

	cout << endl;

	if (result == TIE) { cout << "It's a tie!" << endl; }
	else if (result == HUMAN) { cout << "Player won!" << endl; }
	else if (result == AI) { cout << "AI won!" << endl; }
	else { cout << "System Error!" << endl; }
	
	return false;
}

int GameController::minimax(const std::vector<std::vector<int>> Table, int depth, const int player, const int c, int r, int alpha, int beta)
{
    const int result = isGameEnded(Table, r, c, player);
    int score, move;
    
    if (result != CONT  || depth == 0) {
		if (result != CONT) {
			if (result == HUMAN) { score = INT_MIN; }
			else if (result == AI) { score = INT_MAX; }
			else { score = 0; }
		}
		else { score = evaluateMove(Table, c, player); }

        return score;
    }

    if(player == AI){
        int bestScore = INT_MIN;

        for(move = 1; move <= COLUMN ; move++){
            if(isColumnFree(move, Table)){
                vector<vector<int>> newTable = putPiece(Table, move, &r, HUMAN);
                score = minimax(newTable, depth - 1, HUMAN, move, r, alpha, beta);
                bestScore = max(score, bestScore);
				alpha = max(bestScore, alpha);

				if (beta <= alpha) { break; }
            }
        }

        return bestScore;
    }
    else {
        int bestScore = INT_MAX;

        for(move = 1; move <= COLUMN; move++){
            if(isColumnFree(move, Table)){
                vector<vector<int>> newTable = putPiece(Table, move, &r, AI);
                score = minimax(newTable, depth - 1, AI, move, r, alpha, beta);
                bestScore = min(score, bestScore);

				beta = min(bestScore, beta);

				if (beta <= alpha) { break; }
            }
        }

        return bestScore;
    }
}

int GameController::evaluateMove(const std::vector<std::vector<int>> Table, const int col, const int player)
{
    int score = 0;
/*
	for (int i = 3; i < 6; i++) {
		for (int k = ROW; k > 0; k--) {
			if (Table[k][i] == player){
				if (i == 3) {score += 3;}
				else { score += 2; }
			}
		}
	}
*/
	// Horizontal pieces
	for (int i = 1; i <= COLUMN -  MAX_SPACE_TO_WIN; i++) {
		for (int k = 1; k <= ROW; k++) {
			int adjacent_pieces[4] = 	{Table[k][i], Table[k][i + 1],
										Table[k][i + 2], Table[k][i + 3]};
			score += evaluateAdjacents(adjacent_pieces, player);
		}
	}

	// Vertical pieces
	for (int i = 1; i <= COLUMN; i++) {
		for (int k = 1; k <= ROW - MAX_SPACE_TO_WIN; k++) {
			int adjacent_pieces[4] = 	{Table[k][i], Table[k + 1][i],
										Table[k + 2][i], Table[k + 3][i]};
			score += evaluateAdjacents(adjacent_pieces, player);
		}
	}

	// Diagonal upwards pieces
	for (int i = 1; i <= COLUMN -  MAX_SPACE_TO_WIN; i++) {
		for (int k = 1; k <= ROW - MAX_SPACE_TO_WIN; k++) {
			int adjacent_pieces[4] = 	{Table[k][i], Table[k + 1][i + 1],
										Table[k + 2][i + 2], Table[k + 3][i + 3]};
			score += evaluateAdjacents(adjacent_pieces, player);
		}
	}

	// Diagonal downwards pieces
	for (int i = 1; i <= COLUMN -  MAX_SPACE_TO_WIN; i++) {
		for (int k = MAX_SPACE_TO_WIN + 1; k <= ROW; k++) {
			int adjacent_pieces[4] = 	{Table[k][i], Table[k - 1][i + 1],
										Table[k - 2][i + 2], Table[k - 3][i + 3]};
			score += evaluateAdjacents(adjacent_pieces, player);
		}
	}

    return score;
}

int GameController::evaluateAdjacents(const int *adjacentPieces, const int player)
{
    int opponent = AI;

	if (player == AI) { opponent = HUMAN; }

	int score = 0, playerPieces = 0, emptySpaces = 0, opponentPieces = 0;

	for (int i = 0; i < 4; i++) {
		if (adjacentPieces[i] == player) { playerPieces++; }
		else if(adjacentPieces[i] == 0) { emptySpaces++; }
		else if (adjacentPieces[i] == opponent) { opponentPieces++; }
	}

	if (playerPieces == 4) { score += 9999; }
	else if (playerPieces == 3 && emptySpaces == 1) { score += 100; }
	else if ( playerPieces == 2 && emptySpaces == 2	) { score += 10; }

	if (opponentPieces == 4) { score -= 9999; }
	else if (opponentPieces == 3 && emptySpaces == 1) { score -= 100; }
	else if (opponentPieces == 2 && emptySpaces == 2	) { score -= 10; }

    return score;
}

GameController::~GameController()
{

}
