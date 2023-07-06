/*
 * world.cpp
 */

#include "world.h"
#include "../game.h"
#include "systems/rendering.h"
#include "systems/physics.h"

#define SCUG_SPEED   50

namespace world{
   EntityManager* entity_manager;
}

Entity scug;

System* rendering_system = new RenderingSystem();
System* physics_system = new PhysicsSystem();

World::World() {
   world::entity_manager = new EntityManager(); 

   Image* grangle = game::resource_manager->LoadImage("./res/tex/grangle.jpg");

   scug = world::entity_manager->CreateEntity();
   world::entity_manager->AddComponent(scug, Sprite{grangle});
   world::entity_manager->AddComponent(scug, Transform2D{Vector2{25,25},Vector2{25,25}});
   SDL_Log("scug %d\n", scug);

   //  Entity scug2 = world::entity_manager->CreateEntity();
   //  world::entity_manager->AddComponent(scug2, Sprite{grangle});
   //  world::entity_manager->AddComponent(scug2, Transform2D{Vector2{50,50},Vector2{50,50}});
   //  SDL_Log("scug2 %d\n", scug2);

   Entity scug3 = world::entity_manager->CreateEntity();
   world::entity_manager->AddComponent(scug3, Sprite{grangle});
   world::entity_manager->AddComponent(scug3, Transform2D{Vector2{75,75},Vector2{50,50}});
   SDL_Log("scug3 %d\n", scug3);
   
 

}

void World::Update(){
   rendering_system->Update(game::delta_time);
   physics_system->Update(game::delta_time);  
   
   Vector2* position = &world::entity_manager->GetComponent<Transform2D>(scug)->position;
   if (game::key_state[SDL_SCANCODE_W]) {
      SDL_Log("Up");
      position->y -= SCUG_SPEED*game::delta_time;  // Move up
   }
   if (game::key_state[SDL_SCANCODE_A]) {
      SDL_Log("Left");
      position->x -= SCUG_SPEED*game::delta_time;  // Move left
   }
   if (game::key_state[SDL_SCANCODE_S]) {
      SDL_Log("Down");
      position->y += SCUG_SPEED*game::delta_time;  // Move down
   }
   if (game::key_state[SDL_SCANCODE_D]) {
      SDL_Log("Right");
      position->x += SCUG_SPEED*game::delta_time;  // Move right
   }
}