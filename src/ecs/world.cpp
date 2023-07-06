/*
 * world.cpp
 */

#include "world.h"
#include "../game.h"
#include "systems/rendering.h"

namespace world{
   EntityManager* entity_manager;
}

System* rendering_system = new RenderingSystem();
Entity scug; 

World::World(){
   world::entity_manager = new EntityManager(); 

   Image* grangle = game::resource_manager->LoadImage("./res/tex/grangle.jpg");

   scug = world::entity_manager->CreateEntity();
   world::entity_manager->AddComponent(scug, Sprite{grangle});
   world::entity_manager->AddComponent(scug, Transform2D{Vector2{640/2,400/2},Vector2{25,25}});
}

void World::Update(){
   Vector2* scug_size = &(world::entity_manager->GetComponent<Transform2D>(scug)->size);
   
   *scug_size+=Vector2{50*game::delta_time,5*game::delta_time};



   rendering_system->Update(game::delta_time);
}