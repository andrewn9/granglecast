/*
 * world.cpp
 */

#include "world.h"

#include "../game.h"
#include "systems/rendering.h"
#include "systems/physics.h"
#include "systems/inputs.h"

#include <random>

#define SCUG_SPEED   0.7f

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

bool grounded = false;

void CreateScug() {
   Entity scug = world::entity_manager->CreateEntity();
   world::entity_manager->AddComponent(scug, Sprite{ pobe });

   std::uniform_int_distribution<int> pX(WINDOW_HEIGHT/-2, WINDOW_WIDTH/2);
   std::uniform_int_distribution<int> pY(WINDOW_HEIGHT/-2, WINDOW_HEIGHT/2);
   std::uniform_int_distribution<int> pS(0, 250);
   std::uniform_int_distribution<int> velocity(-100, 100);

   world::entity_manager->AddComponent(scug, 
      Transform2D {
         Vector2{static_cast<float>(pX(generator)), static_cast<float>(pY(generator))}, 
         Vector2{static_cast<float>(pS(generator)), static_cast<float>(pS(generator))}
      }
   );
   world::entity_manager->AddComponent(scug, 
      Collider {
         false,
         50,
         0.8,
         0.8,
         0
      }
   );
   world::entity_manager->AddComponent(scug, 
      Velocity {
         Vector2{static_cast<float>(velocity(generator)), static_cast<float>(velocity(generator))} / 100
      }
   );

   SDL_Log("Scug created. %d", scug);
   scugs.push_back(scug);
}

void CreateScug(float x, float y, float w, float h, bool anchored, float mass) {
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
         anchored, // Anchored
         mass,    // Mass
         0.8,
         0.8,
         0     // Restitution
      }
   );
   world::entity_manager->AddComponent(scug, 
      Velocity {
         Vector2{0,0}
      }
   );

   SDL_Log("Scug created. %d", scug);
   scugs.push_back(scug);
}

void DeleteScug() {
   std::uniform_int_distribution<int> id(0, scugs.size() - 1);
   int rand = id(generator);
   world::entity_manager->RemoveEntity(scugs[rand]);
   SDL_Log("Scug deleted. %d", scugs[rand]);
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
      if (event.keycode == SDLK_e) {
         world::entity_manager->GetComponent<Transform2D>(scugcat)->position = Vector2{0, 0};
         world::entity_manager->GetComponent<Velocity>(scugcat)->velocity = Vector2{0, 0};
      } 
      if (event.keycode == SDLK_1) {
         world::entity_manager->GetComponent<Transform2D>(world::camera)->size = { WINDOW_WIDTH, WINDOW_HEIGHT };
      }
      if (event.keycode == SDLK_2) {
         world::entity_manager->GetComponent<Transform2D>(world::camera)->size = { WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2 };
      }
   }  
}

void HandleMouse(const MouseEvent& event) {
   if (event.type == InputBegan) {
      switch (event.mouse_button) {
         case 1:
            SDL_Log("Left clicked at: %s(%f, %f)", "Vector2", event.position.x, event.position.y);
            CreateScug(event.position.x + world::entity_manager->GetComponent<Transform2D>(world::camera)->position.x - world::entity_manager->GetComponent<Transform2D>(world::camera)->size.x / 2, event.position.y + world::entity_manager->GetComponent<Transform2D>(world::camera)->position.y - world::entity_manager->GetComponent<Transform2D>(world::camera)->size.y/2, 50, 50, false, 10);
            break;
         case 2:
            SDL_Log("Middle clicked at: %s(%f, %f)", "Vector2", event.position.x, event.position.y);
            break;
         case 3:
            SDL_Log("Right clicked at: %s(%f, %f)", "Vector2", event.position.x, event.position.y);
            for (int i = 0; i < 100; i++) {
               CreateScug(event.position.x + world::entity_manager->GetComponent<Transform2D>(world::camera)->position.x - world::entity_manager->GetComponent<Transform2D>(world::camera)->size.x / 2, event.position.y + world::entity_manager->GetComponent<Transform2D>(world::camera)->position.y - world::entity_manager->GetComponent<Transform2D>(world::camera)->size.y/2, 50, 50, false, 10);
            }
            break;
      }
   }
}

void HandleCollision(const CollisionEvent& event) {
   if (event.entity_a == 2){
      if (event.normal.y < -0.707) {
         grounded = true;
      }
   }
}

World::World() {
   world::entity_manager = new EntityManager(); 
   world::event_manager = new EventManager();

   Entity amogus = world::entity_manager->CreateEntity();

   world::camera = world::entity_manager->CreateEntity();
   world::entity_manager->AddComponent(world::camera, Transform2D{Vector2{0,0},Vector2{WINDOW_WIDTH,WINDOW_HEIGHT}});
   
   grangle = game::resource_manager->LoadImage("./res/tex/grangle.jpg");
   pobe = game::resource_manager->LoadImage("./res/tex/pobe.png");

   scugcat = world::entity_manager->CreateEntity();
   world::entity_manager->AddComponent(scugcat, Sprite{ grangle });
   world::entity_manager->AddComponent(scugcat, Transform2D{Vector2{0,0}, Vector2{50,50}});
   world::entity_manager->AddComponent(scugcat, Velocity{Vector2{0,0}});
   world::entity_manager->AddComponent(scugcat, Collider{false, 10, 0.8, 0.8, 0});

   world::event_manager->connect<KeyboardEvent>(HandleKeys);
   world::event_manager->connect<MouseEvent>(HandleMouse);
   world::event_manager->connect<CollisionEvent>(HandleCollision);

   CreateScug(0, 0, 5000, 400, true, 0);
   // CreateScug(200, 0, 10, 400, true, 0);
   // CreateScug(100, 100, 10, 200, true, 0);
}

void World::FixedUpdate() {
   Vector2* velocity = &world::entity_manager->GetComponent<Velocity>(scugcat)->velocity;

   grounded = false;
   
   physics_system->Update();

   // velocity->x = 0;
   // velocity->y = 0;
   if (inputs::held_keys.find(SDLK_w) != inputs::held_keys.end() && grounded) {
      velocity->y -= 15;
   }
   // if (inputs::held_keys.find(SDLK_s) != inputs::held_keys.end()) {
   //    velocity->y = SCUG_SPEED;
   // }
   int dx = 0;

   if (inputs::held_keys.find(SDLK_d) != inputs::held_keys.end())
      dx++;
   if (inputs::held_keys.find(SDLK_a) != inputs::held_keys.end())
      dx--;

   if (grounded) {
      world::entity_manager->GetComponent<Velocity>(scugcat)->velocity.x += dx * SCUG_SPEED;
   } else {
      world::entity_manager->GetComponent<Velocity>(scugcat)->velocity.x += dx * 0.1;
   }

   world::entity_manager->GetComponent<Transform2D>(world::camera)->position = (world::entity_manager->GetComponent<Transform2D>(world::camera)->position).Lerp(world::entity_manager->GetComponent<Transform2D>(scugcat)->position, 0.1f);
}

void World::Update() {
   input_system->Update();
   rendering_system->Update();
}