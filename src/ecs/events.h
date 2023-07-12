/*
 * events.h
 */

#pragma once

struct CollisionEvent {
    Entity entity_a;
    Entity entity_b;
    Vector2 normal;
    float depth;
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