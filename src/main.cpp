/*
 * main.c starts and exits the game
 */

#include "game.h"

#include "ecs/components.h"
#include "ecs/entitymanager.h"
#include "vector2.h"

int main(int argc, char* argv[])
{
    Game game;

    if (game.Initialize())
    {
        game.RunLoop();
    }

    game.Shutdown();
    return 0;
}
