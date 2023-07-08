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

std::vector<Entity> scugs;

System* input_system = new InputSystem();
System* rendering_system = new RenderingSystem();
System* physics_system = new PhysicsSystem();

Image* grangle;

std::random_device rd;
std::mt19937 generator(rd());

void CreateScug() {
    Entity scug = world::entity_manager->CreateEntity();
    world::entity_manager->AddComponent(scug, Sprite{ grangle });

    std::uniform_int_distribution<int> pX(0, WINDOW_WIDTH);
    std::uniform_int_distribution<int> pY(0, WINDOW_HEIGHT);
    std::uniform_int_distribution<int> pS(0, 250);

    world::entity_manager->AddComponent(scug, Transform2D{Vector2{pX(generator), pY(generator)}, Vector2{pS(generator), pS(generator)}});

    SDL_Log("Scug created. %d", scug);
    scugs.push_back(scug);
}

void DeleteScug() {
   std::uniform_int_distribution<int> id(0, scugs.size() - 1);
   Entity entity = scugs[id(generator)];

   SDL_Log("Scug deletion. %d", entity);
   world::entity_manager->RemoveEntity(scugs[entity]);  
}

void HandleKeys(const InputEvent& event) { 
   if (event.type == InputEventType::KeyPress && event.keycode == SDLK_f) {
      world::entity_manager->PrintData();
   }
   if (event.type == InputEventType::KeyPress && event.keycode == SDLK_r) {
      DeleteScug();
   }
   if (event.type == InputEventType::KeyPress && event.keycode == SDLK_t) {
      CreateScug();
   }
   if (event.type == InputEventType::KeyPress && event.keycode == SDLK_1) {
      world::entity_manager->GetComponent<Transform2D>(world::camera)->size = {WINDOW_WIDTH, WINDOW_HEIGHT};
   }
   if (event.type == InputEventType::KeyPress && event.keycode == SDLK_2) {
      world::entity_manager->GetComponent<Transform2D>(world::camera)->size = {WINDOW_WIDTH/2, WINDOW_HEIGHT/2};
   }
}

Entity scugcat;

World::World() {
   world::entity_manager = new EntityManager(); 
   world::event_manager = new EventManager();

   world::camera = world::entity_manager->CreateEntity();
   world::entity_manager->AddComponent(world::camera, Transform2D{Vector2{0,0},Vector2{WINDOW_WIDTH,WINDOW_HEIGHT}});

   grangle = game::resource_manager->LoadImage("./res/tex/grangle.jpg");

   scugcat = world::entity_manager->CreateEntity();
   world::entity_manager->AddComponent(scugcat, Sprite{ grangle });
   world::entity_manager->AddComponent(scugcat, Transform2D{Vector2{0,0}, Vector2{50,50}});

   world::event_manager->connect<InputEvent>(HandleKeys);
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

   world::entity_manager->GetComponent<Transform2D>(world::camera)->size = {sin(game::time) * WINDOW_WIDTH / 2 + WINDOW_WIDTH, cos(game::time) * WINDOW_HEIGHT / 2 + WINDOW_HEIGHT};
   rendering_system->Update();
}