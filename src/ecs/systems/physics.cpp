/*
 * physics.cpp
 */

#include "physics.h"
#include "../../game.h"
#include "../world.h"

void PhysicsSystem::Update() {
    for (Entity entityA : world::entity_manager->GetEntitiesWithComponent<Transform2D>()) {
        if (world::entity_manager->GetComponent<Velocity>(entityA)) {
            Vector2* velocity = &world::entity_manager->GetComponent<Velocity>(entityA)->velocity;
            world::entity_manager->GetComponent<Transform2D>(entityA)->position += *velocity;
            Collider* collider = world::entity_manager->GetComponent<Collider>(entityA);
            
            if (!(collider && collider->anchored)) {
                velocity->y += 1;
            }
        }
        for (Entity entityB : world::entity_manager->GetEntitiesWithComponent<Transform2D>()) {
            if (!world::entity_manager->GetComponent<Collider>(entityA) || !world::entity_manager->GetComponent<Collider>(entityB)) {
                continue;
            }
            if(entityA != entityB) {
                AABB(entityA, entityB);
            }
        }
    }
}

void PhysicsSystem::AABB(Entity entity, Entity other) {
    Transform2D* transformA = world::entity_manager->GetComponent<Transform2D>(entity);
    Transform2D* transformB = world::entity_manager->GetComponent<Transform2D>(other);

    Vector2 a_half_widths = transformA->size/2;
    Vector2 b_half_widths = transformB->size/2;

    Vector2 diff = transformA->position - transformB->position;

    Vector2 overlap = a_half_widths + b_half_widths - Vector2{abs(diff.x), abs(diff.y)};

    //SAT test for the x-axis and y-axis
    if (overlap.x > 0 && overlap.y > 0) {
        //intersection on both axes, get normal and fire collision event

        CollisionEvent collisionEvent;
        collisionEvent.entityA = entity;
        collisionEvent.entityB = other;

        if (overlap.x > overlap.y) {
            collisionEvent.normal = (diff.y > 0)? Vector2{0, 1} : Vector2{0, -1};
            collisionEvent.depth = overlap.y;
        } else {
            collisionEvent.normal = (diff.x > 0)? Vector2{1, 0} : Vector2{-1, 0};
            collisionEvent.depth = overlap.x;
        }
        if (entity == 1) {
            SDL_Log("x: %f y : %f", collisionEvent.normal.x, collisionEvent.normal.y);
        }
        world::event_manager->fire(collisionEvent);
    }

}
