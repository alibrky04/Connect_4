#ifndef __GAME_TABLE__
#define __GAME_TABLE__

#include <vector>

class Game_Table {
private:
	std::vector <std::vector <int>> Table; // A vector that holds the game's table
public:
	Game_Table(); // Constructor for the class

	std::vector <std::vector <int>> getTable(); // Method that returns Table attribute

	void setTable(std::vector <std::vector <int>> P_Table); // Method that for changing Table attribute

	int getRowNumber(); // Method that returns ROW constant

	int getColumnNumber(); // Method that returns COLUMN constant

	void printTable(); // Method that prints out the Table attribute to the screen 

	~Game_Table(); // Destructor for the class
};

#endif // !__GAME_TABLE__
