# Planetary-Lander-Simulator
The planetary lander is a game which uses differential equations (ODEs) and solves them in real-time in order to simulate the lander getting to a planet. Keeps highscores of players.

# Description

This game presents the user with a menu to run a simulation/play the game or check the scores.
Once the game starts, the user will be directed on which keys to press to either slow down or accelarate the planetary lander. The goal of the game is to reach the surface of the destination planet, in this case Mars. This has to be done without bouncing, this means that the lander has to reach the surface at an appropriate acceleration. Otherwise, the lander will 'bounce' away from the surface.

# Installation

## Windows
Note: This project was done using VS Code, for other editors this process might differ.

If you are compiling this code in Windows you will need to follow the steps below:

+ Setup c_cpp_properties.json

https://www.40tude.fr/blog/compile-cpp-code-with-vscode-clang/

Once you this file is generated the program can be compiled by running the following commands

```bash
mkdir build
g++ main.cpp -o ./build/game
./build/game.exe
```
