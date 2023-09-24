/*
 * rendering.h
 */

#pragma once

#include "../system.h"

class RenderingSystem : public System {
    public:
        void Initialize();
        void Update();
        void Draw(Entity entity);

    private:
        uint shader_program;
};
