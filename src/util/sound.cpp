
#include "sound.h"

Sound::Sound(const char* path) {
    chunk = Mix_LoadWAV(path);
    if (!chunk) {
        SDL_Log("Failed to load sound: %s", path);
    }
}
