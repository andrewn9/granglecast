/*
 *  entitymanager.cpp 
 *  Contains methods for the creation and removal of entities.
 */

#include "entitymanager.h"
#include "world.h"
#include <string>

// Create a new entity
Entity EntityManager::CreateEntity() {
    // If there are no inactive entities, create a new one
    if (!inactive_entities.empty()) {
        // Get the first inactive entity
        Entity new_entity = inactive_entities.front();
        // Pop it from the list
        inactive_entities.pop();
        // Return the new entity
        return new_entity;
    }

    // If there are no more entities to create, create a new one
    if (next_entity < MAX_ENTITIES) {
        // Create a new entity
        Entity new_entity = next_entity;
        // Increment the next entity
        next_entity++;
        // Return the new entity
        return new_entity;
    }

    // If there are no more entities to create, return 0
    return 0;
}

// Remove an entity
void EntityManager::RemoveEntity(Entity entity) {
    // If the entity is 0, return
    if (entity == 0) {
        return;
    }

    // Iterate through each component type
    for (auto& [component_type, component_set] : component_data[entity]) {
        // Remove the entity from the component set
        component_sets[component_type].erase(entity);
    }
    // Remove the entity from the component data
    component_data.erase(entity);

    // Push the entity back into the inactive list
    inactive_entities.push(entity);
}

// Print the data
void EntityManager::PrintData() const {
    // Iterate through each entity
    for (const auto& [entity, components] : component_data) {
        // Log the entity ID
        SDL_Log("Entity ID: %u", entity);
        // Iterate through each component type
        for (const auto& [typeIndex, component] : components) {
            // Log the component type ID
            SDL_Log("\tComponent Type ID: %s", typeIndex.name());
        }
        // Log a new line
        SDL_Log("");
    }
}