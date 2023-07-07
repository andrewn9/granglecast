/*
 * inputs.cpp
 */

#include "inputs.h"
#include "../../game.h"
#include "../world.h"

namespace inputs {
    std::unordered_set<SDL_Keycode> held_keys;
}

void InputSystem::Update() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                game::is_running = false;
                break;
            case SDL_KEYDOWN: {
                // Held keys does not contain the key
                if (inputs::held_keys.find(e.key.keysym.sym) == inputs::held_keys.end()) {
                    HandleKeyEvent(e.key.keysym.sym, InputEventType::KeyPress);
                }

                break;
            }
            case SDL_KEYUP: {
                // Held keys does contain the key
                if (inputs::held_keys.find(e.key.keysym.sym) != inputs::held_keys.end()) {
                    HandleKeyEvent(e.key.keysym.sym, InputEventType::KeyRelease);
                }
                break;
            }
        }
    }
}

// Initializes and fires key event
void InputSystem::HandleKeyEvent(SDL_Keycode key, InputEventType type) {
    InputEvent key_event;
    key_event.type = type;
    key_event.keycode = key;

    if(type==InputEventType::KeyPress) {
        world::event_manager->fire(key_event);
        inputs::held_keys.insert(key);
    } else if(type==InputEventType::KeyRelease) {
        world::event_manager->fire(key_event);
        inputs::held_keys.erase(key);
    }
}