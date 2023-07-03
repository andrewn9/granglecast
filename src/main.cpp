/*
 * main.c starts and exits the game
 */

#include "game.h"
#include "vector2.hpp"

int main(int argc, char* argv[])
{
    Game game;

    Vector2 *vector = new Vector2(1, 2);

    printf("x :%f y:%f\n", (*vector).x, vector -> y);

    if (game.Initialize())
    {
        game.RunLoop();
    }

    game.Shutdown();
    return 0;
}
