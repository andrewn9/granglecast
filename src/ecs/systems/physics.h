/*
 * physics.h
 */


#pragma once

#include "../system.h"

class PhysicsSystem : public System {
    public:
        void Update();
        void AABB(Entity entityA, Entity entityB);
};