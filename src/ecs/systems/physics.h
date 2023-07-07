/*
 * physics.h
 */


#pragma once

#include "../system.h"

class PhysicsSystem : public System {
    public:
        void Update();
        bool AABB(Entity entityA, Entity entityB);
};
