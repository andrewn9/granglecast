/*
 * physics.h
 */


#pragma once

#include "../system.h"

class PhysicsSystem : public System {
    public:
        void Update();
        void AABB(Entity entity_a, Entity entity_b);
        void ResolveCollision(const CollisionEvent& collision_event);
};