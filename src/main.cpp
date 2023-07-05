
/*
 * main.cpp is the entry point of the game.
 * It initializes the Game object, runs the game loop, and shuts down the game.
 */

#include "game.h"

int main(int argc, char* argv[])
{
    Game game;

    // Check if game initialized successfully
    if (game.Initialize()) {
        game.RunLoop();
    }

    // Exit
    game.Shutdown();
    return 0;
}
