/*
 *  system.h
 *  Serves as an interface for updating the system and provides access to the EntityManager for entity management.
 */

#pragma once

#include "entitymanager.h"  
#include "events.h"

class System {
    public:
        virtual void Update() = 0; // Mark function as purely virtual
};
