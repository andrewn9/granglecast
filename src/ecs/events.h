/*
 * events.h
 */

#pragma once

struct CollisionEvent {
    Entity entityA;
    Entity entityB;
};

enum InputEventType {
    InputBegan,
    InputEnded
};

struct KeyboardEvent {
    InputEventType type;
    SDL_Keycode keycode;
};

struct MouseEvent {
    InputEventType type;
    Vector2 position;
    Uint8 mouse_button;
};