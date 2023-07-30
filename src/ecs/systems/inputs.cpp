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
                // Held keys do not contain the key
                if (inputs::held_keys.find(e.key.keysym.sym) == inputs::held_keys.end()) {
                    HandleKeyEvent(e.key.keysym.sym, InputBegan);
                }
                break;
            }
            case SDL_KEYUP: {
                // Held keys contain the key
                if (inputs::held_keys.find(e.key.keysym.sym) != inputs::held_keys.end()) {
                    HandleKeyEvent(e.key.keysym.sym, InputEnded);
                }
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                HandleMouseEvent(e.button.button, InputBegan, e.button.x, e.button.y);
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                HandleMouseEvent(e.button.button, InputEnded, e.button.x, e.button.y);
                break;
            }
            case SDL_MOUSEMOTION: {
                break;
            }
        }
    }
}

// Initializes and fires key event
void InputSystem::HandleKeyEvent(SDL_Keycode key, InputEventType type) {
    
    KeyboardEvent key_event;
    key_event.type = type;
    key_event.keycode = key;

    if (type == InputBegan) {
        world::event_manager->fire(key_event);
        inputs::held_keys.insert(key);
    } else if (type == InputEnded) {
        world::event_manager->fire(key_event);
        inputs::held_keys.erase(key);
    }
}

// Initializes and fires mouse event
void InputSystem::HandleMouseEvent(Uint8 button, InputEventType type, int x, int y) {
    MouseEvent mouse_event;
    mouse_event.type = type;
    mouse_event.mouse_button = button;
    mouse_event.position = Vector2{static_cast<float>(x), static_cast<float>(y)};
    world::event_manager->fire(mouse_event);
}

