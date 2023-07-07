/*
 * events.h
 */

#pragma once

struct CollisionEvent {
    Entity entityA;
    Entity entityB;
};

enum class InputEventType {
    KeyPress,
    KeyRelease
};

struct InputEvent {
    InputEventType type;
    SDL_Keycode keycode;
};