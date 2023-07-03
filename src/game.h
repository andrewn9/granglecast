/*
 * game.h contains functions for initialization/shutdown of game window, and contains the game loop.
 */

#pragma once

#include <SDL2/SDL.h>
#include "resourcemanager.h"

#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   400

#define WINDOW_NAME     "sick window"

class Game
{
public:
    Game();

    // Initializes SDL_Window and SDL_Renderer, SDL_Init(), ResourceManager
    bool Initialize();

    // Processes inputs and draws screen
    void RunLoop();

    // Frees all resources and safely exit game
    void Shutdown();

private:
    ResourceManager* manager;
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
};
