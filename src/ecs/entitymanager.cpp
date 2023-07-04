/*
 * entitymanager.cpp 
 */

#include "entitymanager.h"

Entity EntityManager::CreateEntity() {
    // Keep entities in range
    if (entities >= MAX_ENTITIES) {
        return -1;
    }

    // Initialize an empty component map for the entity
    components[entities] = {}; 
    return ++entities;
}

// TODO: Removing entities... new entities should replace the ids of old