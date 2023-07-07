/*
 * entitymanager.h 
 * Holds the EntityManager class, which manages entities and their components.
 * Contains methods for the creation, addition, retrieval, and removal of components associated with entities.
 */

#pragma once

#include <algorithm>
#include <cstdint>
#include <typeindex>
#include <vector>
#include <queue>
#include <unordered_map>

#include "components.h"

typedef uint32_t Entity;

class EntityManager {
    public:
        // Returns a newly created entity with a unique id
        Entity CreateEntity();

        // Removes entity and its components, id is placed in queue
        void RemoveEntity(Entity entity);

        // Associates component with entity
        template<typename Component>
        void AddComponent(Entity entity, Component&& component);

        // Remove component associated with entity
        template<typename Component>
        void RemoveComponent(Entity entity);

        void RemoveComponentByType(Entity entity, const std::type_index& component_type);
        
        // Gets component associated with entity
        template<typename Component>
        Component* GetComponent(Entity entity) const;

        // Returns all entities with the specified component
        template<typename Component>
        const std::vector<Entity>& GetEntitiesWithComponent() const;
        
        // Prints formatted entity structures
        void PrintData() const;

    private:
        const uint32_t MAX_ENTITIES = 100;

        uint32_t entities = 0;
        
        // Queue of ids to be reused after an entity is removed
        std::queue<Entity> inactive_entities;

        // The entity is the key. At each entity pair, is a pair of the component type index and the actual component struct.
        std::unordered_map<Entity, std::unordered_map<std::type_index, void*>> components;

        // The component type is the key, with a vector of all entities that posseses it 
        std::unordered_map<std::type_index, std::vector<Entity>> component_index;

};

template<typename Component>
void EntityManager::AddComponent(Entity entity, Component&& component) {
    std::type_index component_type = typeid(Component);
    
    // Map the component to the entity
    components[entity][component_type] = new Component(std::forward<Component>(component));

    // Add component to its corrosponding pool
    component_index[component_type].push_back(entity);
}

template<typename Component>
const std::vector<Entity>& EntityManager::GetEntitiesWithComponent() const {
    std::type_index component_type = typeid(Component);
    
    // Check if the component type exists in the index
    auto it = component_index.find(component_type);
    if (it != component_index.end()) {
        // Component type was found
        return it->second;
    }
    
    // If the component type is not found, return an empty vector
    static const std::vector<Entity> empty;
    return empty;
}

template<typename Component>
Component* EntityManager::GetComponent(Entity entity) const {
    std::type_index component_type = typeid(Component);

    // Check if the entity exists
    auto entity_iterator = components.find(entity);
    if (entity_iterator != components.end()) {
        // Check if the component type exists for the entity
        auto component_iterator = entity_iterator->second.find(component_type);
        if (component_iterator != entity_iterator->second.end()) {
            // Component type found, cast the component pointer using static_cast
            return static_cast<Component*>(component_iterator->second);
        }
    }

    // If the component or entity is not found, return nullptr
    return nullptr;
}

// Removes a component associated with an entity
template<typename Component>
void EntityManager::RemoveComponent(Entity entity)
{
    std::type_index component_type = typeid(Component);

    // Check if the entity exists
    auto entity_iterator = components.find(entity);
    if (entity_iterator != components.end())
    {
        // Check if the component type exists for the entity
        auto component_iterator = entity_iterator->second.find(component_type);
        if (component_iterator != entity_iterator->second.end())
        {
            // Component type found, delete the component and remove it from the entity's components
            delete static_cast<Component*>(component_iterator->second);
            entity_iterator->second.erase(component_iterator);

            // Remove the entity from the component's index
            auto index_iterator = component_index.find(component_type);
            if (index_iterator != component_index.end())
            {
                auto& entities = index_iterator->second;
                entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
            }
        }
    }
}
