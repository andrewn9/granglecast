
#include "image.h"

Image::Image(const char* path, SDL_Renderer* renderer) {
    surface = IMG_Load(path);
    if (!surface) {
        SDL_Log("Failed to load image: %s", path);
        return;
    }

    this->renderer = renderer;
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_Log("Failed to create texture from surface: %s", path);
        SDL_FreeSurface(surface);
        return;
    }

    srect.w = surface->w;
    srect.h = surface->h;
}







