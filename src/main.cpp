#include "game.h"
#include "vector2.hpp"
#include "manager.hpp"
#include <stdio.h>

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
