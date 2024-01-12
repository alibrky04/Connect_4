#include <iostream>
#include <vector>
#include "functions.h"
#include "Game_Table.h"

#define ROW 6
#define COLUMN 7
#define MOD 4

using namespace std;

void p2_game_loop()
{
	Game_Table game_table;

	int player = 2, chosen_column, row = 0;

	do {
		if (player == 1)
			player = 2;
		else
			player = 1;

		game_table.printTable();

		if (player == 1 || player == 2) {
			chosen_column = columnQuestion(player, game_table.getTable());
			game_table.setTable(putPiece(game_table.getTable(), chosen_column, &row, player));
		}
		else {
			cout << "Player count is broken!" << endl;
			break;
		}

		std::cout << std::endl;
	} while (!isGameEnded(game_table.getTable(), row, chosen_column, player));

	game_table.printTable();

	std::cout << std::endl;

	cout << "Player " << player << " won!" << endl;
}

bool isGameEnded(const std::vector<std::vector<int>> Table, const int row, const int column, const int player)
{
	int counter = 0, new_row = row, new_column = column;
	const int control_array_row[4] = { 0, -1, -1, -1 }, control_array_column[4] = { -1, 0, -1, 1 }; // row, column, 135 degrees, 45 degrees

	for (int i = 0; i < 4; i++) {
		for (int k = 0; Table[row + control_array_row[i] + k * control_array_row[i]][column + control_array_column[i] + k * control_array_column[i]] == player; k++) {
			new_row = row + control_array_row[i] + k * control_array_row[i];
			new_column = column + control_array_column[i] + k * control_array_column[i];
		}

		for (int k = 0; k < MOD; k++) {
			if (Table[new_row - k * control_array_row[i]][new_column - k * control_array_column[i]] == player) {
				counter++;
			}
			else {
				break;
			}

			if (counter == MOD) {
				return true;
			}
		}

		counter = 0;
		new_row = row, new_column = column;
	}

	return false;
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
			*row = i;
			Table[i][column] = player;
			break;
		}
	}

	return Table;
}
