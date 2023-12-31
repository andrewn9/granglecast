/*
 *  game.cpp
 *  Implementation of the Game class, representing the main game logic and loop.
 *  It initializes the game, processes events, and manages the game loop.
 */

#include "game.h"

namespace game {
    ResourceManager* resource_manager = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    double delta_time = 1.0f/60;
    bool is_running = true;
    double const physics_time = 1.0f/60;
    double time = 0.0f;
}

bool Game::Initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Initialize window
    game::window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!game::window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // Initialize renderer
    game::renderer = SDL_CreateRenderer(game::window, -1, SDL_RENDERER_ACCELERATED  | SDL_RENDERER_PRESENTVSYNC );
    if (!game::renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    // Initialize resource_manager and SDL_IMG
    game::resource_manager = new ResourceManager(game::renderer);

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_Log("Failed to initialize SDL_image: %s", IMG_GetError());
        return false;
    }

    world = new World();
    
    return true;
}

// Initiate update loop
void Game::RunLoop() {
    Uint64 last_tick = SDL_GetPerformanceCounter();
    float accumulator;
    while (game::is_running) {
        game::delta_time = (double) (SDL_GetPerformanceCounter() - last_tick) / SDL_GetPerformanceFrequency();
        last_tick = SDL_GetPerformanceCounter();
        game::time += game::delta_time;

        accumulator += game::delta_time;

        while(accumulator >= game::physics_time) {
            world->FixedUpdate();
            accumulator -= game::physics_time;
        } 

        world->Update();
    }
    Shutdown();
}

// Exit game
void Game::Shutdown() {
    game::resource_manager->Close();
    SDL_DestroyRenderer(game::renderer);
    SDL_DestroyWindow(game::window);
    SDL_Quit();
}
