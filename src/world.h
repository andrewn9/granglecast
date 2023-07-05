/*
 * game.h contains functions for initialization/shutdown of game window, and contains the game loop.
 */

#pragma once

#include <vector>
#include <SDL2/SDL.h>

#include "game.h"
#include "ecs/entitymanager.h"
#include "ecs/system.h"
#include "resourcemanager.h"

#define MAX_ENTITIES    100

class EntityManager;
class World {
    public:
        World();
        void update();
    private:
        EntityManager* entityManager;
        std::vector<System*> systems;
};
