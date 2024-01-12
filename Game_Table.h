#ifndef __GAME_TABLE__
#define __GAME_TABLE__

#include <vector>

class Game_Table {
private:
	std::vector <std::vector <int>> Table;
public:
	Game_Table();

	std::vector <std::vector <int>> getTable();

	void setTable(std::vector <std::vector <int>> P_Table);

	int getRowNumber();

	int getColumnNumber();

	void printTable();

	~Game_Table();
};

#endif // !__GAME_TABLE__
