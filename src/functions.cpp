#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "functions.h"
#include "Game_Table.h"

#define ROW 6
#define COLUMN 7
#define MOD 4
#define HUMAN 1
#define AI 2
#define TIE 0
#define CONT 3

using namespace std;

void p2_game_loop()
{
	Game_Table game_table;

	int player = 2, chosen_column, row = 0, result;

	do {
		if (player == 1)
			player = 2; // Changes player turn to the second player
		else
			player = 1; // Changes player turn to the first player

		game_table.printTable();

		if (player == 1 || player == 2) {
			chosen_column = columnQuestion(player, game_table.getTable()); // Gets the column from the player
			game_table.setTable(putPiece(game_table.getTable(), chosen_column, &row, player)); // Changes the table according to the chosen column
		}
		else {
			cout << "Player count is broken!" << endl;
			break;
		}

		cout << endl;

		result = isGameEnded(game_table.getTable(), row, chosen_column, player);
	} while (result == CONT); // Checks if the game has ended

	game_table.printTable();

	cout << endl;

	if(result == TIE){
		cout << "it's a tie!" << endl;
	}
	else{
		cout << "Player " << result << " won!" << endl;
	}
}

// Main solver function for solving if the game has ended or not
// Controls for directions of the last put piece
// Goes to the end of a direction to control all possibilities
int isGameEnded(const std::vector<std::vector<int>> Table, const int row, const int column, const int player)
{
	int counter = 0, new_row = row, new_column = column; // counter is for counting the number of pieces, new_row and new_column are for going to the end of a direction
	const int control_array_row[4] = { 0, -1, -1, -1 }, control_array_column[4] = { -1, 0, -1, 1 }; // row, column, 135 degrees, 45 degrees

	for (int i = 0; i < 4; i++) {
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

int columnQuestion(int player, const std::vector<std::vector<int>> Table)
{
	int column_choice;

	do {
		cout << "Where would you want to drop your piece? You are player " << player << endl;
		cout << "Your Choice: ";
		cin >> column_choice;
		if (!isColumnFree(column_choice, Table)) {
			cout << column_choice << ". column is full!" << endl;
		}
	} while (!isColumnFree(column_choice, Table));

	return column_choice;
}

bool isColumnFree(int column, const std::vector<std::vector<int>> Table)
{
	for (int i = 1; i <= ROW; i++) {
		if (Table[i][column] == 0) {
			return true;
		}
	}
	return false;
}

std::vector <std::vector <int>> putPiece(std::vector<std::vector<int>> Table, int column, int* row, int player)
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

int minimax(std::vector<std::vector<int>> Table, int depth, int player, int c, int r)
{
    const int scores[3] = { 0, -1, 1 }; // Check the pointing system. Might be wrong.

    int result = isGameEnded(Table, r, c, player);
    int score, move;
    
    if (result != CONT) {
        score = scores[result];
        return score;
    }

    if(player == AI){
        int bestScore = INT_MIN;

        for(move = 1; move < 8; move++){
            if(isColumnFree(move, Table)){
                vector<vector<int>> newTable = putPiece(Table, move, &r, HUMAN);
                score = minimax(newTable, depth + 1, HUMAN, move, r);
                bestScore = max(score, bestScore);
            }
        }

        return bestScore;
    }
    else {
        int bestScore = INT_MAX;

        for(move = 1; move < 8; move++){
            if(isColumnFree(move, Table)){
                vector<vector<int>> newTable = putPiece(Table, move, &r, AI);
                score = minimax(newTable, depth + 1, AI, move, r);
                bestScore = min(score, bestScore);
            }
        }

        return bestScore;
    }
}

void ai_game_loop()
{
	srand(time(NULL));

	Game_Table game_table;

	int player = AI, chosen_column, row = 0, result;

	do {
		if (player == HUMAN)
			player = AI; // Changes player turn to the ai
		else
			player = HUMAN; // Changes player turn to the human

		game_table.printTable();

		if (player == HUMAN) {
			chosen_column = columnQuestion(player, game_table.getTable()); // Gets the column from the player
			game_table.setTable(putPiece(game_table.getTable(), chosen_column, &row, player)); // Changes the table according to the chosen column
		}
		else if (player == AI) {
			int bestScore = INT_MIN, bestMove, score;

			for(chosen_column = 1; chosen_column < 8; chosen_column++){
				if(isColumnFree(chosen_column, game_table.getTable())){
					vector<vector<int>> newTable = putPiece(game_table.getTable(), chosen_column, &row, player);
					score = minimax(newTable, 0, player, chosen_column, row);

					if(score > bestScore){
						bestScore = score;
						bestMove = chosen_column;
					}
				}
			}

			chosen_column = bestMove;
			game_table.setTable(putPiece(game_table.getTable(), chosen_column, &row, player));
		}
		else {
			cout << "Player count is broken!" << endl;
			break;
		}

		cout << endl;

		result = isGameEnded(game_table.getTable(), row, chosen_column, player);
	} while (result == CONT); // Checks if the game has ended

	game_table.printTable();

	cout << endl;

	switch (result)
	{
	case TIE:
		cout << "It's a tie!" << endl;
		break;
	
	case HUMAN:
		cout << "Player won!" << endl;
		break;

	case AI:
		cout << "AI won!" << endl;
		break;

	default:
		cout << "System Error!" << endl;
		break;
	}
}
