#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Image {
    public:
        SDL_Surface* surface;
        SDL_Texture* texture;

        SDL_Rect srect = {0, 0, 0, 0};

        SDL_Renderer* renderer;

        Image(const char* path, SDL_Renderer* renderer);
};