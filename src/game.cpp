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
    SDL_GLContext gl_context = nullptr;
    double delta_time = 1.0f/60;
    bool is_running = true;
    double const physics_time = 1.0f/60;
    double time = 0.0f;
}

using namespace game;

bool Game::Initialize() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    // Initialize window
    window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    if (!window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        SDL_Log("Failed to create GL context: %s", SDL_GetError());
        return false;
    }

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		SDL_Log("Failed to initialize GLEW.");
		return false;
	}

    // Initialize renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED  | SDL_RENDERER_PRESENTVSYNC );
    if (!renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    // Initialize resource_manager and SDL_IMG
    resource_manager = new ResourceManager(renderer);

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
    while (is_running) {
        delta_time = (double) (SDL_GetPerformanceCounter() - last_tick) / SDL_GetPerformanceFrequency();
        last_tick = SDL_GetPerformanceCounter();
        time += delta_time;

        accumulator += delta_time;

        while(accumulator >= physics_time) {
            world->FixedUpdate();
            accumulator -= physics_time;
        } 

        world->Update();
    }
    Shutdown();
}

// Exit game
void Game::Shutdown() {
    resource_manager->Close();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
