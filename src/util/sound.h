#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

class Sound {
    public:
        Mix_Chunk* chunk;

        Sound(const char* path);
};
