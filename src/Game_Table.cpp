#include <iostream>
#include <vector>
#include "Game_Table.h"

#define ROW 6
#define COLUMN 7

Game_Table::Game_Table() : Table(8, std::vector<int>(9, 0))
{
	
}

std::vector <std::vector <int>> Game_Table::getTable()
{
	return Table;
}

void Game_Table::setTable(std::vector <std::vector <int>> P_Table)
{
	Table = P_Table;
}

int Game_Table::getRowNumber()
{
	return ROW;
}

int Game_Table::getColumnNumber()
{
	return COLUMN;
}

void Game_Table::printTable()
{
	for (int j = 1; j <= COLUMN; j++) {
		std::cout << j << "  ";
	}

	std::cout << std::endl;
	
	for (int j = 1; j <= COLUMN; j++) {
		std::cout << "-" << "  ";
	}

	std::cout << std::endl;

	for (int i = 1; i <= ROW; i++) {
		for (int j = 1; j <= COLUMN; j++) {
			if (Table[i][j] == 1) {
				std::cout << 'x' << "  ";
			}
			else if (Table[i][j] == 2) {
				std::cout << 'o' << "  ";
			}
			else {
				std::cout << '|' << "  ";
			}
		}
		std::cout << std::endl;
	}
}

Game_Table::~Game_Table()
{
}
