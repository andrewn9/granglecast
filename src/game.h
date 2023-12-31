/*
 * game.h
 * Defines the game class which represents the main game logic and loop.
 */

#pragma once

#include <cstdint>
#include <functional>
#include <SDL2/SDL.h>

#include "resourcemanager.h"
#include "ecs/world.h"
#include "ecs/entitymanager.h"

#define WINDOW_WIDTH    640
#define WINDOW_HEIGHT   400

#define WINDOW_NAME     "granglecast"

namespace game {
    extern ResourceManager* resource_manager;
    extern SDL_Window* window;
    extern SDL_Renderer* renderer;
    extern double delta_time;
    extern double const physics_time;
    extern bool is_running;
    extern double time;
}

class Game {
    public:
        // Initializes SDL_Window and SDL_Renderer, SDL_Init(), ResourceManager
        bool Initialize();

        // Processes inputs and draws screen
        void RunLoop();

        // Frees all resources and safely exit game
        void Shutdown();

    private:
        World* world;
};

