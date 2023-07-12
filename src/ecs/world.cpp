/*
 * world.cpp
 */

#include "world.h"

#include "../game.h"
#include "systems/rendering.h"
#include "systems/physics.h"
#include "systems/inputs.h"

#include <random>

#define SCUG_SPEED   50

namespace world{
   EntityManager* entity_manager;
   EventManager* event_manager;
   Entity camera;
}

Entity scugcat;
std::vector<Entity> scugs;

System* input_system = new InputSystem();
System* rendering_system = new RenderingSystem();
System* physics_system = new PhysicsSystem();

Image* grangle;
Image* pobe;

std::random_device rd;
std::mt19937 generator(rd());

void CreateScug() {
   Entity scug = world::entity_manager->CreateEntity();
   world::entity_manager->AddComponent(scug, Sprite{ pobe });

   std::uniform_int_distribution<int> pX(WINDOW_HEIGHT/-2, WINDOW_WIDTH/2);
   std::uniform_int_distribution<int> pY(WINDOW_HEIGHT/-2, WINDOW_HEIGHT/2);
   std::uniform_int_distribution<int> pS(0, 250);

   world::entity_manager->AddComponent(scug, 
      Transform2D {
         Vector2{static_cast<float>(pX(generator)), static_cast<float>(pY(generator))}, 
         Vector2{static_cast<float>(pS(generator)), static_cast<float>(pS(generator))}
      }
   );
   world::entity_manager->AddComponent(scug, 
      Collider {
         true,
         0,
         0
      }
   );

   SDL_Log("Scug created. %d", scug);
   scugs.push_back(scug);
}

void CreateScug(float x, float y, float w, float h) {
   Entity scug = world::entity_manager->CreateEntity();
   world::entity_manager->AddComponent(scug, Sprite{ pobe });

   world::entity_manager->AddComponent(scug, 
      Transform2D {
         Vector2{x, y}, 
         Vector2{w, h}
      }
   );
   world::entity_manager->AddComponent(scug, 
      Collider {
         true,
         0,
         0
      }
   );

   SDL_Log("Scug created. %d", scug);
   scugs.push_back(scug);
}

void DeleteScug() {
   std::uniform_int_distribution<int> id(0, scugs.size() - 1);
   int rand = id(generator);
   world::entity_manager->RemoveEntity(scugs[rand]);
   scugs.erase(scugs.begin() + rand);
}

void HandleKeys(const KeyboardEvent& event) {
   if (event.type == InputBegan) {
      if (event.keycode == SDLK_f) {
         world::entity_manager->PrintData();
      }
      if (event.keycode == SDLK_r) {
         DeleteScug();
      }
      if (event.keycode == SDLK_t) {
         CreateScug();
      } 
      if (event.keycode == SDLK_1) {
         world::entity_manager->GetComponent<Transform2D>(world::camera)->size = { WINDOW_WIDTH, WINDOW_HEIGHT };
      }
      if (event.keycode == SDLK_2) {
         world::entity_manager->GetComponent<Transform2D>(world::camera)->size = { WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 };
      }
   }  
}

void HandleMouse(const MouseEvent& event) {
   if (event.type == InputBegan) {
      switch (event.mouse_button) {
         case 1:
            SDL_Log("Left clicked at: %s(%f, %f)", "Vector2", event.position.x, event.position.y);
            break;
         case 2:
            SDL_Log("Middle clicked at: %s(%f, %f)", "Vector2", event.position.x, event.position.y);
            break;
         case 3:
            SDL_Log("Right clicked at: %s(%f, %f)", "Vector2", event.position.x, event.position.y);
            break;
      }
   }
}

void HandleCollision(const CollisionEvent& event) {
   if (event.entityA == 1){
      world::entity_manager->GetComponent<Transform2D>(event.entityA)->position += event.normal * event.depth;
      if (event.normal.y != 0) {
         world::entity_manager->GetComponent<Velocity>(event.entityA)->velocity = Vector2{world::entity_manager->GetComponent<Velocity>(event.entityA)->velocity.x, 0};
      }
   }
}

World::World() {
   world::entity_manager = new EntityManager(); 
   world::event_manager = new EventManager();

   world::camera = world::entity_manager->CreateEntity();
   world::entity_manager->AddComponent(world::camera, Transform2D{Vector2{0,0},Vector2{WINDOW_WIDTH,WINDOW_HEIGHT}});

   grangle = game::resource_manager->LoadImage("./res/tex/grangle.jpg");
   pobe = game::resource_manager->LoadImage("./res/tex/pobe.png");

   scugcat = world::entity_manager->CreateEntity();
   world::entity_manager->AddComponent(scugcat, Sprite{ grangle });
   world::entity_manager->AddComponent(scugcat, Transform2D{Vector2{0,0}, Vector2{50,50}});
   world::entity_manager->AddComponent(scugcat, Velocity{Vector2{0,0}});
   world::entity_manager->AddComponent(scugcat, Collider{false, 0, 0});

   world::event_manager->connect<KeyboardEvent>(HandleKeys);
   world::event_manager->connect<MouseEvent>(HandleMouse);
   world::event_manager->connect<CollisionEvent>(HandleCollision);

   CreateScug(-100, 100, 200, 20);
}

void World::FixedUpdate() {
   physics_system->Update();
   world::entity_manager->GetComponent<Transform2D>(world::camera)->position = (world::entity_manager->GetComponent<Transform2D>(world::camera)->position).Lerp(world::entity_manager->GetComponent<Transform2D>(scugcat)->position, 0.1f);
}

void World::Update() {
   input_system->Update();

   Vector2* position = &world::entity_manager->GetComponent<Transform2D>(scugcat)->position;

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