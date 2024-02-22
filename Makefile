all:
	g++ -I src/include -L src/lib -o main src/main.cpp src/functions.cpp src/Game_Table.cpp -lmingw32 -lSDL2main -lSDL2