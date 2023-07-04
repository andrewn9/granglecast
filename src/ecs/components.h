/*
 * components.h contains all of the structs used as components in the EntityManager and systems
 */

#pragma once

#include "../util/vector2.h"

struct Transform2D {
    Vector2 position;
    Vector2 size;
};