/*
 * world.h
 */

#pragma once

#include <vector>
#include <SDL2/SDL.h>

#include "components.h"
#include "entitymanager.h"
#include "system.h"

#include "events.h"
#include "eventmanager.h"

namespace world {
    extern EntityManager* entity_manager;
    extern EventManager* event_manager;
    extern Entity camera;
}

class World {
    public:
        World();
        void Update();
        void FixedUpdate();
};

