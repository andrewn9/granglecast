/*
 * rendering.h
 */

#pragma once

#include "../system.h"

class RenderingSystem : public System {
    public:
        void Update();
        void Draw(Entity entity);
};
