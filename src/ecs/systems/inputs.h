/*
 * inputs.h
 */

#pragma once

#include <SDL2/SDL.h>
#include <unordered_set>

#include "../system.h"

namespace inputs {
    extern std::unordered_set<SDL_Keycode> held_keys;
}

class InputSystem : public System {
    public:
        void Update();
    private:
        void HandleKeyEvent(SDL_Keycode key, InputEventType type);
};

