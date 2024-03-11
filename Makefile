all:
	g++ -I src/include -L src/lib -o main src/main.cpp src/Game_Table.cpp src/GameController.cpp src/SDLController.cpp -lmingw32 -lSDL2main -lSDL2