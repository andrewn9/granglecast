/*
 * world.h
 */

#pragma once

#include <vector>
#include <SDL2/SDL.h>

#include "entitymanager.h"
#include "system.h"
#include "components.h"

namespace world {
    extern EntityManager* entity_manager;
}

class World {
    public:
        World();
        void Update();
    private:
        std::vector<System*> systems;
};

