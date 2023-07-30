/*
 * inputs.h
 */

#pragma once

#include <SDL2/SDL.h>
#include <unordered_set>

#include "../system.h"


namespace inputs {
    // Set of held keys
    extern std::unordered_set<SDL_Keycode> held_keys;
}

// Handle input events
class InputSystem {
    public:
        // Update input system
        void Update();
        
    private:
        // Handle a key event
        void HandleKeyEvent(SDL_Keycode key, InputEventType type);

        // Handle a mouse event
        void HandleMouseEvent(Uint8 button, InputEventType type, int x, int y);
};

