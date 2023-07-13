/*
 * physics.cpp
 */

#include "physics.h"
#include "../../game.h"
#include "../world.h"

void PhysicsSystem::Update() {
    for (Entity entity_a : world::entity_manager->GetEntitiesWithComponent<Transform2D>()) {
        if (world::entity_manager->GetComponent<Velocity>(entity_a)) {
            Vector2* velocity = &world::entity_manager->GetComponent<Velocity>(entity_a)->velocity;
            Collider* collider = world::entity_manager->GetComponent<Collider>(entity_a);
            
            // Ravity
            if (!(collider && collider->anchored)) {
                world::entity_manager->GetComponent<Transform2D>(entity_a)->position += *velocity;
                velocity->y += 0.5;
            }
        }
        for (Entity entity_b : world::entity_manager->GetEntitiesWithComponent<Transform2D>()) {
            if (!world::entity_manager->GetComponent<Collider>(entity_a) || !world::entity_manager->GetComponent<Collider>(entity_b)) {
                continue;
            }
            if(entity_a != entity_b) {
                AABB(entity_a, entity_b);
            }
        }
    }
}

void PhysicsSystem::AABB(Entity entity_a, Entity entity_b) {
    Transform2D* transformA = world::entity_manager->GetComponent<Transform2D>(entity_a);
    Transform2D* transformB = world::entity_manager->GetComponent<Transform2D>(entity_b);

    Vector2 a_half_widths = transformA->size/2;
    Vector2 b_half_widths = transformB->size/2;

    Vector2 diff = transformA->position - transformB->position;

    Vector2 overlap = a_half_widths + b_half_widths - Vector2{abs(diff.x), abs(diff.y)};

    //SAT test for the x-axis and y-axis
    if (overlap.x > 0 && overlap.y > 0) {
        //intersection on both axes, get normal and fire collision event

        CollisionEvent collision_event;
        collision_event.entity_a = entity_a;
        collision_event.entity_b = entity_b;

        if (overlap.x > overlap.y) {
            collision_event.normal = (diff.y > 0)? Vector2{0, 1} : Vector2{0, -1};
            collision_event.depth = overlap.y;
        } else {
            collision_event.normal = (diff.x > 0)? Vector2{1, 0} : Vector2{-1, 0};
            collision_event.depth = overlap.x;
        }

        world::event_manager->fire(collision_event);
        ResolveCollision(collision_event);
    }
}

void PhysicsSystem::ResolveCollision(const CollisionEvent& collision_event) {
    Entity entity_a = collision_event.entity_a;
    Entity entity_b = collision_event.entity_b;

    Collider& collider_a = *world::entity_manager->GetComponent<Collider>(entity_a);
    Collider& collider_b = *world::entity_manager->GetComponent<Collider>(entity_b);

    Velocity& velocity_a = *world::entity_manager->GetComponent<Velocity>(entity_a);
    Velocity& velocity_b = *world::entity_manager->GetComponent<Velocity>(entity_b);

    // Calculate relative velocity
    Vector2 relative_velocity = velocity_b.velocity - velocity_a.velocity;

    // Calculate relative velocity in terms of the normal direction
    float velAlongNormal = relative_velocity.DotProduct(collision_event.normal);
    
    // Do not resolve if velocities are separating
    if(velAlongNormal <= 0)
        return;

    // Calculate restitution
    float e = std::min(collider_a.restitution, collider_b.restitution);

    float a_inv_mass = collider_a.mass == 0 ? 0 : 1 / collider_a.mass;
    float b_inv_mass = collider_b.mass == 0 ? 0 : 1 / collider_b.mass;
    
    float j = -(1.0 + e) * velAlongNormal;
    j /= a_inv_mass + b_inv_mass;

    // Apply impulse
    Vector2 impulse = collision_event.normal * j;
    
    velocity_a.velocity -= impulse * a_inv_mass;
    velocity_b.velocity += impulse * b_inv_mass;
    
    const float percent = 0.5; // usually 20% to 80%
    Vector2 correction = collision_event.normal * ((collision_event.depth / (a_inv_mass + b_inv_mass)) * percent);

    world::entity_manager->GetComponent<Transform2D>(entity_a)->position += correction * a_inv_mass;
    world::entity_manager->GetComponent<Transform2D>(entity_b)->position -= correction * b_inv_mass;
}
