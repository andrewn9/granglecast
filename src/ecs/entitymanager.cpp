/*
 *  entitymanager.cpp 
 *  Contains methods for the creation and removal of entities.
 */

#include "entitymanager.h"

// Returns a newly created entity with a unique id 
Entity EntityManager::CreateEntity() {
    // Keep entities in rangec
    if (entities >= MAX_ENTITIES - 1) {
        return -1;
    }

    // Initialize an empty component map for the entity
    components[entities] = {};  
    return entities++;
}

// TODO: Removing entities... new entities should replace the ids of old