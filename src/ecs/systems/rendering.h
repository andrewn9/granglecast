/*
 * rendering.h
 */

#pragma once

#include "../system.h"

class RenderingSystem {
    public:
        // Update the rendering system
        void Update();
        // Draw the given sprite and transform
        void Draw(Sprite* sprite, Transform2D* transform);
};