/*
 * resourcemanager.cpp contains definitions of sounds and images, with methods to load/store sounds and images,
 * as well as clean up upon destruction. 
 *
 * Each resource is stored in an unordered map with the file path as the key.
 */

#include "resourcemanager.h"

// Initialize SDL_Mixer and open audio channel
ResourceManager::ResourceManager(SDL_Renderer* renderer) {
    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) {
        SDL_Log("Failed to initialize SDL_mixer");
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        SDL_Log("Failed to open audio: %s", Mix_GetError());
    }

    this->renderer = renderer;
}

// Loads sound from specified path
Sound* ResourceManager::LoadSound(const char* path) {
    Sound* sound = new Sound(path);
    if (sound && sound->chunk) {
        sounds[path] = sound;
        return sound;
    } else {
        delete sound;
        SDL_Log("Failed to load sound: %s", path);
        return nullptr;
    }
}

// Loads image from specified path
Image* ResourceManager::LoadImage(const char* path) {
    Image* image = new Image(path, renderer);
    if (image && image->surface && image->texture) {
        images[path] = image;
        return image;
    } else {
        delete image;
        SDL_Log("Failed to load image: %s", path);
        return nullptr;
    }
}

// Free all resources
void ResourceManager::Close() {
    for (const auto& [key, sound] : sounds) {
        if (sound && sound->chunk) {
            Mix_FreeChunk(sound->chunk);
        }
        delete sound;
    }

    Mix_CloseAudio();
    Mix_Quit();

    for (const auto& [key, image] : images) {
        if (image) {
            SDL_FreeSurface(image->surface);
            SDL_DestroyTexture(image->texture);
        }
        delete image;
    }

    IMG_Quit();
}
