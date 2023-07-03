#pragma once

#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>

class Sound
{
public:
    Mix_Chunk *chunk;

    Sound(char const *path) 
    {
        chunk = Mix_LoadWAV(path);
    }

    void Play() 
    {
        Mix_PlayChannel(-1, chunk, 0);
    }
};

class Image
{
public:
    SDL_Surface *surface;
    SDL_Texture *texture;

    SDL_Rect srect = {0, 0, 0, 0};
    SDL_Rect drect = {0, 0, 0, 0};

    SDL_Renderer *ren;

    Image(char const *path, SDL_Renderer *renderer)
    {
        surface = IMG_Load(path);
        ren = renderer;
        texture = SDL_CreateTextureFromSurface(ren, surface);
        srect.w = surface->w;
        srect.h = surface->h;
        drect.w = surface->w;
        drect.h = surface->h;
    }

    void Render()
    {
        SDL_RenderCopy(ren, texture, &srect, &drect);
    }
};

class ResourceManager
{
private:
    std::unordered_map<std::string, Sound*> sounds;
    std::unordered_map<std::string, Image*> images;
    SDL_Renderer *ren;
public:
    ResourceManager(SDL_Renderer *renderer) 
    {
        Mix_Init(MIX_INIT_MP3);

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            
        }

        ren = renderer;
    };

    Sound* LoadSound(char const *path)
    {
        Sound *sound = new Sound(path);
        sounds[path] = sound;
        return sound;
    }

    Image* LoadImage(char const *path)
    {
        Image *image = new Image(path, ren);
        images[path] = image;
        return image;
    }

    void Close()
    {
        for (const auto & [key, sound] : sounds) 
        {
            Mix_FreeChunk(sound -> chunk);
        }

        Mix_CloseAudio();
        Mix_Quit();
        
        for (const auto & [key, image] : images) 
        {
            SDL_FreeSurface(image->surface);
            SDL_DestroyTexture(image->texture);
        }

        IMG_Quit();
    }
};