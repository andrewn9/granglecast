/*
 * resourcemanager.h contains definitions of sounds and images, with methods to load/store sounds and images,
 * as well as clean up upon destruction. 
 *
 * Each resource is stored in an unordered map with the file path as the key.
 */
 
#pragma once

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include <unordered_map>
#include <string>

class Sound {
    public:
        Mix_Chunk* chunk;

        Sound(const char* path);
        void Play();
};

class Image {
    public:
        SDL_Surface* surface;
        SDL_Texture* texture;

        SDL_Rect srect = {0, 0, 0, 0};
        SDL_Rect drect = {0, 0, 0, 0};

        SDL_Renderer* renderer;

        Image(const char* path, SDL_Renderer* renderer);
        void Render();
};

class ResourceManager {
    private:

        // Resource maps with file paths as keys
        std::unordered_map<std::string, Sound*> sounds;
        std::unordered_map<std::string, Image*> images;

        SDL_Renderer* renderer;

    public:
        ResourceManager(SDL_Renderer* renderer);
        
        // Image and sound initialization methods
        Sound* LoadSound(const char* path);
        Image* LoadImage(const char* path);

        // Frees all resources and exits
        void Close();
};

