#include "physics.h"
#include "../../game.h"

void PhysicsSystem::Update(float delta_time) {

    for (Entity entityA : world::entity_manager->GetEntitiesWithComponent<Transform2D>()) {
        
        for (Entity entityB : world::entity_manager->GetEntitiesWithComponent<Transform2D>()) {
            
            if(entityA != entityB) {
                if(AABB(entityA, entityB)) {
                    SDL_Log("%d is colliding with %d", entityA, entityB);
                }
            }
        }
    }

}

bool PhysicsSystem::AABB(Entity entity, Entity other) {
    Transform2D* transformA = world::entity_manager->GetComponent<Transform2D>(entity);
    Transform2D* transformB = world::entity_manager->GetComponent<Transform2D>(other);

    Vector2 positionA = transformA->position;
    Vector2 sizeA = transformA->size;
    Vector2 positionB = transformB->position;
    Vector2 sizeB = transformB->size;

    Vector2 minA = positionA - (sizeA/2);
    Vector2 maxA = positionA + (sizeA/2);
    Vector2 minB = positionB - (sizeB/2);
    Vector2 maxB = positionB + (sizeB/2);

    bool xOverlap = maxA.x >= minB.x && minA.x <= maxB.x;
    bool yOverlap = maxA.y >= minB.y && minA.y <= maxB.y;

    return xOverlap && yOverlap;
}
