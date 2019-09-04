# Planetary-Lander-Simulator
The planetary lander is a game which uses differential equations (ODEs) and solves them in real-time in order to simulate the lander getting to a planet. Keeps highscores of players.


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

## Linux
