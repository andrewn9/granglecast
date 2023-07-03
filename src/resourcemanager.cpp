/*
 * resourcemanager.cpp contains definitions of sounds and images, with methods to load/store sounds and images,
 * as well as clean up upon destruction. 
 *
 * Each resource is stored in an unordered map with the file path as the key.
 */

#include "resourcemanager.h"

Sound::Sound(const char* path)
{
    chunk = Mix_LoadWAV(path);
    if (!chunk)
    {
        SDL_Log("Failed to load sound: %s", path);
    }
}

void Sound::Play()
{
    if (chunk)
    {
        Mix_PlayChannel(-1, chunk, 0);
    }
}

Image::Image(const char* path, SDL_Renderer* renderer)
{
    surface = IMG_Load(path);
    if (!surface)
    {
        SDL_Log("Failed to load image: %s", path);
        return;
    }

    ren = renderer;
    texture = SDL_CreateTextureFromSurface(ren, surface);
    if (!texture)
    {
        SDL_Log("Failed to create texture from surface: %s", path);
        SDL_FreeSurface(surface);
        return;
    }

    srect.w = surface->w;
    srect.h = surface->h;
    drect.w = surface->w;
    drect.h = surface->h;
}

void Image::Render()
{
    if (ren && texture)
    {
        SDL_RenderCopy(ren, texture, &srect, &drect);
    }
}

ResourceManager::ResourceManager(SDL_Renderer* renderer)
{
    if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3)
    {
        SDL_Log("Failed to initialize SDL_mixer");
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        SDL_Log("Failed to open audio: %s", Mix_GetError());
    }

    ren = renderer;
}

Sound* ResourceManager::LoadSound(const char* path)
{
    Sound* sound = new Sound(path);
    if (sound && sound->chunk)
    {
        sounds[path] = sound;
        return sound;
    }
    else
    {
        delete sound;
        SDL_Log("Failed to load sound: %s", path);
        return nullptr;
    }
}

Image* ResourceManager::LoadImage(const char* path)
{
    Image* image = new Image(path, ren);
    if (image && image->surface && image->texture)
    {
        images[path] = image;
        return image;
    }
    else
    {
        delete image;
        SDL_Log("Failed to load image: %s", path);
        return nullptr;
    }
}


void ResourceManager::Close()
{
    for (const auto& [key, sound] : sounds)
    {
        if (sound && sound->chunk)
        {
            Mix_FreeChunk(sound->chunk);
        }
        delete sound;
    }

    Mix_CloseAudio();
    Mix_Quit();

    for (const auto& [key, image] : images)
    {
        if (image)
        {
            SDL_FreeSurface(image->surface);
            SDL_DestroyTexture(image->texture);
        }
        delete image;
    }

    IMG_Quit();
}
