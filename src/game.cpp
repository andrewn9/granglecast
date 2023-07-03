/*
 * game.cpp contains functions for initialization/shutdown of game window, and contains the game loop.
 */

#include "game.h"

Game::Game() : window(nullptr), renderer(nullptr), isRunning(true) {}

Sound *snare;
Image *grangle;

bool Game::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(WINDOW_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        SDL_Log("Failed to initialize SDL_image: %s", IMG_GetError());
        return false;
    }

    manager = new ResourceManager(renderer);
    snare = manager -> LoadSound("./res/snd/snare.wav");
    grangle = manager -> LoadImage("./res/tex/grangle.jpg");

    // Create entities and components
    // ...

    return true;
}

void Game::RunLoop()
{
    while (isRunning)
    {
        // Process events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
			{
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				// Method #1 for getting keyboard input:
				// Checking for SDL_KEYDOWN or SDL_KEYUP events
				switch (event.key.keysym.scancode)
				{
				case SDL_SCANCODE_W:
					snare -> Play();
                    break;
                default:
                    break;
                }
                break;
		    }
            // Add additional event handling here if needed
        }

        // Process input
        // ...

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        grangle->Render();

        SDL_RenderPresent(renderer);
    }
}

void Game::Shutdown()
{
    manager -> Close();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
