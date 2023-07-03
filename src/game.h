#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "manager.hpp"

#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   400

#define WINDOW_NAME     "sick window"

class Game
{
public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();

private:
    ResourceManager* manager;
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;
};
