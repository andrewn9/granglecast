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
    uint32_t deltaTime = 0;
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
    game::renderer = SDL_CreateRenderer(game::window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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


    // snare = game::resource_manager->LoadSound("./res/snd/snare.wav");
    // grangle = game::resource_manager->LoadImage("./res/tex/grangle.jpg");

    // Create entities and components
    // ...


    // entityManager = new EntityManager();

    // Entity entity1 = entityManager->CreateEntity();
    // Entity entity2 = entityManager->CreateEntity();
    // Entity entity3 = entityManager->CreateEntity();
    
    // entityManager->AddComponent(entity1, Transform2D{{0.0f, 0.0f}, {0.0f, 0.0f}});
    // entityManager->AddComponent(entity2, Transform2D{{0.0f, 0.0f}, {0.0f, 0.0f}});

    // const std::vector<Entity>& entitiesWithTransform = entityManager->GetEntitiesWithComponent<Transform2D>();

    // for (Entity entity : entitiesWithTransform) {
    //     SDL_Log("Entity with Transform2D: %d", entity);
    // }

    // Transform2D* transform1 = entityManager->GetComponent<Transform2D>(entity1);

    return true;
}

void Game::RunLoop() {
    uint32_t last_tick_time = 0;

    while (is_running) {
        // Process events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    is_running = false;
                    break;
                case SDL_KEYDOWN:
                    // Method #1 for getting keyboard input:
                    // Checking for SDL_KEYDOWN or SDL_KEYUP events
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_W:
                            //snare->Play();
                            break;
                        default:
                            break;
                    }
                    break;
		    }
        }

        //  world.update();

        // SDL_SetRenderDrawColor(game::renderer, 0, 0, 0, 255);
        // SDL_RenderClear(game::renderer);

        // // grangle->Render();

        // SDL_RenderPresent(game::renderer);
    }
}

// Exit game
void Game::Shutdown() {
    game::resource_manager->Close();
    SDL_DestroyRenderer(game::renderer);
    SDL_DestroyWindow(game::window);
    SDL_Quit();
}
