/*
 *  entitymanager.cpp 
 *  Contains methods for the creation and removal of entities.
 */

#include "entitymanager.h"
#include <string>

// Returns a newly created entity with a unique id 
Entity EntityManager::CreateEntity() {

    if (inactive_entities.empty()) {
        // Keep entities in range
        if (entities < MAX_ENTITIES) {
            // Initialize an empty component map for the entity
            components[entities] = {};  
            return entities++;
        } else {
            return -1;
        }
    } else {
        Entity new_id = inactive_entities.front();
        components[new_id] = {};
        
        // Remove from recycle queue
        inactive_entities.pop();
        return new_id;
    }
}

void EntityManager::RemoveEntity(Entity entity) {
    // Store the component types to be removed
    std::vector<std::type_index> componentTypesToRemove;

    // Find the entity in the components map
    auto entityIterator = components.find(entity);
    if (entityIterator != components.end()) {
        // Iterate over the component map and store the component types
        auto& componentMap = entityIterator->second;
        for (const auto& componentPair : componentMap) {
            const std::type_index& componentType = componentPair.first;
            componentTypesToRemove.push_back(componentType);
        }
    }

    for (const std::type_index& componentType : componentTypesToRemove) {
        RemoveComponentByType(entity, componentType);
    }

    // Remove the entity itself
    components.erase(entity);

    // Recycle id
    inactive_entities.push(entity);
}


void EntityManager::RemoveComponentByType(Entity entity, const std::type_index& component_type) {
    // Check if the entity exists
    auto entity_iterator = components.find(entity);
    if (entity_iterator != components.end()) {
        // Check if the component type exists for the entity
        auto component_iterator = entity_iterator->second.find(component_type);
        if (component_iterator != entity_iterator->second.end()) {
            // Component type found, delete the component and remove it from the entity's components
            entity_iterator->second.erase(component_iterator);

            // Remove the entity from the component's index
            auto index_iterator = component_index.find(component_type);
            if (index_iterator != component_index.end()) {
                auto& entities = index_iterator->second;
                entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
            }
        }
    }
}


void EntityManager::PrintData() const {
    // Print components
    SDL_Log("Components:");
    for (const auto& entityPair : components) {
        Entity entity = entityPair.first;
        const auto& componentMap = entityPair.second;
        for (const auto& componentPair : componentMap) {
            std::type_index component_type = componentPair.first;
            void* component = componentPair.second;
            SDL_Log("Entity: %u | Type: %s | Component: %p\n", entity, component_type.name(), component);
        }
    }
    // Print component_index
    SDL_Log("ComponentIndex:");
    for (const auto& indexPair : component_index) {
        std::type_index component_type = indexPair.first;
        const auto& entities = indexPair.second;

        std::string entitiesString;
        for (const auto& entity : entities) {
            entitiesString += std::to_string(entity) + ", ";
        }

        if (!entitiesString.empty()) {
            entitiesString = entitiesString.substr(0, entitiesString.length() - 2); // Remove the last comma and space
        }

        SDL_Log("Type: %s | Entities: %s", component_type.name(), entitiesString.c_str());
    }
}