/*
 *  components.h
 *  Contains the definitions of the structs used as components in the EntityManager and systems.
 */

#pragma once

#include "../util/vector2.h"
#include "../util/image.h"

// Transform2D: Holds the position and size vectors for a 2D transform
struct Transform2D {
    Vector2 position;
    Vector2 size;
};

struct Sprite {
    Image* image;
};