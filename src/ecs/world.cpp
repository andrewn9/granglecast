/*
 * world.cpp
 */

#include "world.h"

#include "../game.h"
#include "systems/rendering.h"
#include "systems/physics.h"
#include "systems/inputs.h"
#include <math.h>

#define SCUG_SPEED   50

namespace world{
   EntityManager* entity_manager;
   EventManager* event_manager;
   Entity camera;
}

Entity scug, scug2;

System* input_system = new InputSystem();
System* rendering_system = new RenderingSystem();
System* physics_system = new PhysicsSystem();

World::World() {
   world::entity_manager = new EntityManager(); 
   world::event_manager = new EventManager();

   world::camera = world::entity_manager->CreateEntity();
   world::entity_manager->AddComponent(world::camera, Transform2D{Vector2{0,0},Vector2{0,0}});

   Image* grangle = game::resource_manager->LoadImage("./res/tex/grangle.jpg");

   scug = world::entity_manager->CreateEntity();
   world::entity_manager->AddComponent(scug, Sprite{grangle});
   world::entity_manager->AddComponent(scug, Transform2D{Vector2{25,25},Vector2{25,25}});
   //SDL_Log("scug %d\n", scug);

   scug2 = world::entity_manager->CreateEntity();
   world::entity_manager->AddComponent(scug2, Sprite{grangle});
   world::entity_manager->AddComponent(scug2, Transform2D{Vector2{75,75},Vector2{50,50}});
   //world::entity_manager->AddComponent(scug2, Velocity{Vector2{5,5}});
   //SDL_Log("scug2 %d\n", scug2);
   
   // world::event_manager->connect<CollisionEvent>([](const CollisionEvent& event) {
   //    SDL_Log("Collision between %d and %d", event.entityA, event.entityB);
   // });

   // world::event_manager->connect<InputEvent>([](const InputEvent& event) {
   //    if (event.type == InputEventType::KeyPress) {
   //       SDL_Log("%s pressed!", SDL_GetKeyName(event.keycode));
   //    } else if (event.type == InputEventType::KeyRelease) {
   //       SDL_Log("%s released!", SDL_GetKeyName(event.keycode));
   //    }
   // });
}

void World::FixedUpdate() {
   physics_system->Update();

   world::entity_manager->GetComponent<Transform2D>(world::camera)->position = (world::entity_manager->GetComponent<Transform2D>(world::camera)->position).Lerp(world::entity_manager->GetComponent<Transform2D>(scug)->position, 0.1f);
}

void World::Update() {
   input_system->Update();

   Vector2* position = &world::entity_manager->GetComponent<Transform2D>(scug)->position;

   if (inputs::held_keys.find(SDLK_w) != inputs::held_keys.end()) {

      position->y -= SCUG_SPEED*game::delta_time;
   }
   if (inputs::held_keys.find(SDLK_s) != inputs::held_keys.end()) {

      position->y += SCUG_SPEED*game::delta_time;
   }
   if (inputs::held_keys.find(SDLK_d) != inputs::held_keys.end()) {

      position->x += SCUG_SPEED*game::delta_time;
   }
   if (inputs::held_keys.find(SDLK_a) != inputs::held_keys.end()) {

      position->x -= SCUG_SPEED*game::delta_time;
   }

   rendering_system->Update();
}