/*
 *  system.h
 *  Serves as an interface for updating the system and provides access to the EntityManager for entity management.
 */

#pragma once

#include "entitymanager.h"

class System {
    public:
        virtual void Update(float delta_time) = 0;
};