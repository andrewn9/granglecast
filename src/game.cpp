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
    double delta_time = 0;
    const Uint8 *key_state;
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
    game::renderer = SDL_CreateRenderer(game::window, -1, SDL_RENDERER_ACCELERATED /* | SDL_RENDERER_PRESENTVSYNC */);
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

	game::key_state = SDL_GetKeyboardState(NULL);

    world = new World();

    return true;
}

void Game::RunLoop() {
    Uint64 last_tick = SDL_GetPerformanceCounter();

    while (is_running) {

        game::delta_time = (double) (SDL_GetPerformanceCounter() - last_tick) / SDL_GetPerformanceFrequency();
        last_tick = SDL_GetPerformanceCounter();
        
        // Process events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    is_running = false;
                    break;
		    }
        }

        world->Update();
    }
}

// Exit game
void Game::Shutdown() {
    game::resource_manager->Close();
    SDL_DestroyRenderer(game::renderer);
    SDL_DestroyWindow(game::window);
    SDL_Quit();
}
