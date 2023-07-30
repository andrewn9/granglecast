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

using namespace world;

Entity scugcat;
std::vector<Entity> scugs;

InputSystem* input_system = new InputSystem();
RenderingSystem* rendering_system = new RenderingSystem();
PhysicsSystem* physics_system = new PhysicsSystem();

Image* grangle;
Image* pobe;

bool grounded = true;

std::random_device rd;
std::mt19937 generator(rd());

void CreateScug(float x, float y, float w, float h, bool anchored, float mass) {
   Entity scug = entity_manager->CreateEntity();

   entity_manager->AddComponent(scug, Sprite{ pobe });
   entity_manager->AddComponent(scug, 
      Transform2D {
         Vector2{x, y}, 
         Vector2{w, h}
      }
   );
   entity_manager->AddComponent(scug, 
      Velocity {
         Vector2{0,0}
      }
   );
   entity_manager->AddComponent(scug, 
      Collider {
         anchored,
         mass,
         0,
         Vector2{w, h}
      }
   );
   
   SDL_Log("Scug created. %d", scug);
   scugs.push_back(scug);
}

void DeleteScug() {
   if(!scugs.empty()){
      std::uniform_int_distribution<int> id(0, scugs.size() - 1);
      int rand = id(generator);
      entity_manager->RemoveEntity(scugs[rand]);
      SDL_Log("Scug deleted. %d", scugs[rand]);
      scugs.erase(scugs.begin() + rand);
   }
}

void HandleKeys(const KeyboardEvent& event) {
   if (event.type == InputBegan) {
      if (event.keycode == SDLK_f) {
         entity_manager->PrintData();
      }
      if (event.keycode == SDLK_r) {
         DeleteScug();
      }
      if (event.keycode == SDLK_t) {
         CreateScug(0,0,0,0,true,0);
      } 
      if (event.keycode == SDLK_e) {
         entity_manager->GetComponent<Transform2D>(scugcat)->position = Vector2{0, 0};
         entity_manager->GetComponent<Velocity>(scugcat)->velocity = Vector2{0, 0};
      } 
      if (event.keycode == SDLK_1) {
         entity_manager->GetComponent<Transform2D>(camera)->size = { WINDOW_WIDTH, WINDOW_HEIGHT };
      }
      if (event.keycode == SDLK_2) {
         entity_manager->GetComponent<Transform2D>(camera)->size = { WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2 };
      }
   }  
}

void HandleMouse(const MouseEvent& event) {
   if (event.type == InputBegan) {
      switch (event.mouse_button) {
         case 1:
         {
            SDL_Log("Left clicked at: %s(%f, %f)", "Vector2", event.position.x, event.position.y);
            Transform2D& c_transform = *entity_manager->GetComponent<Transform2D>(camera);
            CreateScug((event.position.x * (c_transform.size.x / WINDOW_WIDTH) + c_transform.position.x - c_transform.size.x / 2), (event.position.y* (c_transform.size.y / WINDOW_HEIGHT) + c_transform.position.y - c_transform.size.y/2) , 50, 50, false, 10);
            break;
         }
         case 2:
            SDL_Log("Middle clicked at: %s(%f, %f)", "Vector2", event.position.x, event.position.y);
            break;
         case 3:
            SDL_Log("Right clicked at: %s(%f, %f)", "Vector2", event.position.x, event.position.y);
            for (int i = 0; i < 100; i++) {
               CreateScug(event.position.x + entity_manager->GetComponent<Transform2D>(camera)->position.x - entity_manager->GetComponent<Transform2D>(camera)->size.x / 2, event.position.y + entity_manager->GetComponent<Transform2D>(camera)->position.y - entity_manager->GetComponent<Transform2D>(camera)->size.y/2, 50, 50, false, 10);
            }
            break;
      }
   }
}

void HandleCollision (const CollisionEvent& event) {
   if (event.entity_a == scugcat && event.entity_b == 3) {
      grounded = true;
   }
}


World::World() {
   entity_manager = new EntityManager(); 
   event_manager = new EventManager();

   camera = entity_manager->CreateEntity();
   entity_manager->AddComponent(camera, Transform2D{Vector2{0,0},Vector2{WINDOW_WIDTH,WINDOW_HEIGHT}});
   
   grangle = game::resource_manager->LoadImage("./res/tex/grangle.jpg");
   pobe = game::resource_manager->LoadImage("./res/tex/pobe.png");

   scugcat = entity_manager->CreateEntity();
   entity_manager->AddComponent(scugcat, Sprite{ grangle });
   entity_manager->AddComponent(scugcat, Transform2D{Vector2{0,0}, Vector2{50,50}});
   entity_manager->AddComponent(scugcat, Velocity{Vector2{0,0}});
   entity_manager->AddComponent(scugcat, Collider{true, 10, 0, Vector2{50,50}});

   event_manager->connect<KeyboardEvent>(HandleKeys);
   event_manager->connect<MouseEvent>(HandleMouse);
   event_manager->connect<CollisionEvent>(HandleCollision);

   Entity scugcat2 = entity_manager->CreateEntity();
   entity_manager->AddComponent(scugcat2, Sprite{ grangle });
   entity_manager->AddComponent(scugcat2, Transform2D{Vector2{150,0}, Vector2{50,50}});
   entity_manager->AddComponent(scugcat2, Velocity{Vector2{0,0}});
   entity_manager->AddComponent(scugcat2, Collider{true, 0, 0, Vector2{50,50}});

   Entity scugcat3 = entity_manager->CreateEntity();
   entity_manager->AddComponent(scugcat3, Sprite{ grangle });
   entity_manager->AddComponent(scugcat3, Transform2D{Vector2{150, -150}, Vector2{50,50}});
   entity_manager->AddComponent(scugcat3, Velocity{Vector2{0,10}});
   entity_manager->AddComponent(scugcat3, Collider{true, 10, 0, Vector2{50,50}});
}

void World::FixedUpdate() {
   Vector2* velocity = &entity_manager->GetComponent<Velocity>(scugcat)->velocity;

   grounded = false;
   
   physics_system->Update();

   // velocity->x = 0;
   // velocity->y = 0;
 
   // if (inputs::held_keys.find(SDLK_s) != inputs::held_keys.end()) {
   //    velocity->y = SCUG_SPEED;
   // }
   int dx = 0;
   int dy = 0;

   if (inputs::held_keys.find(SDLK_d) != inputs::held_keys.end())
      dx++;
   if (inputs::held_keys.find(SDLK_a) != inputs::held_keys.end())
      dx--;
   if (inputs::held_keys.find(SDLK_s) != inputs::held_keys.end())
      dy++;
   if (inputs::held_keys.find(SDLK_w) != inputs::held_keys.end())
      dy--;

   entity_manager->GetComponent<Velocity>(scugcat)->velocity.x += dx * 0.1;
   entity_manager->GetComponent<Velocity>(scugcat)->velocity.y += dy * 0.1;

   entity_manager->GetComponent<Transform2D>(camera)->position = (entity_manager->GetComponent<Transform2D>(camera)->position).Lerp(entity_manager->GetComponent<Transform2D>(scugcat)->position, 0.1f);
}

void World::Update() {
   input_system->Update();
   rendering_system->Update();
}