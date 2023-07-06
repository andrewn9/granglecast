#pragma once

#include "../system.h"

class PhysicsSystem : public System {
    public:
        void Update(float delta_time);
        bool AABB(Entity entity, Entity other);
};
