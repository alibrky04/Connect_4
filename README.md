# Connect 4 Game

This is a simple Connect 4 game implemented in C++. Players take turns dropping their colored discs from the top into a vertically suspended grid. The first player to connect four of their discs in a row horizontally, vertically, or diagonally wins the game.

## How to Play

**Compile the Code:**
- Ensure you have a C++ compiler installed on your system.
- Compile the code using the following command:

```markdown
  makefile
```

or

```markdown
  g++ -I src/include -L src/lib -o main src/main.cpp src/Game_Table.cpp src/GameController.cpp src/SDLController.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
```

**Run the Game:**
- Execute the compiled program:

```markdown
  ./main
```

**Gameplay:**
- Players will take turns selecting the column where they want to drop their disc.
- The game will display the updated grid after each move.
- The first player to connect four discs in a row wins the game.

## Features

- **Simple and Intuitive Interface:** The game has a straightforward GUI for ease of use.
- **Two-Player Mode:** Play against a friend on the same computer.
- **AI Mode** Play against a AI that will challenge you to a medium level difficulty battle.
- **Winning Conditions:** The game detects and announces the winner when four discs are connected.

## Code Structure

- **main.cpp:** Main C++ source code starting the game loop.
- **Game_Table.h:** Header file containing the Game_Table class and its methods.
- **Game_Table.cpp:** Source file containing Game_Table class implementation.
- **GameController.h** Header file containing the GameController class and its methods.
- **GameController.cpp** Source file containing GameController class implementation.
- **SDLController.h** Header file containing the SDLController class and its methods.
- **SDLController.cpp** Source file containing SDLController class implementation.

## Contributing

If you would like to contribute to this project, feel free to fork the repository and submit a pull request. Bug reports and suggestions are also welcomed!
