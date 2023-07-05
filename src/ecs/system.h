/*
 *  system.h
 *  Serves as an interface for updating the system and provides access to the EntityManager for entity management.
 */

#pragma once

#include <vector>
#include "entitymanager.h"

class EntityManager;
class System {
    public:
        // Update function to be overriden
        virtual void update(float deltaTime) = 0;

    protected:
        EntityManager* entityManager;
};