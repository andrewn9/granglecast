#pragma once

#include "../system.h"

class RenderingSystem : public System {
    public:
        void Update(float delta_time);
        void Draw(Entity entity);
};
