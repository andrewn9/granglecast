/*
 * entitymanager.h 
 * Holds the EntityManager class, which manages entities and their components.
 * Contains methods for the creation, addition, retrieval, and removal of components associated with entities.
 */

#pragma once

#include <cstdint>
#include <typeindex>
#include <vector>
#include <unordered_map>
#include "components.h"
#include "../game.h"

typedef uint32_t Entity;

class EntityManager {
    public:
        // Returns a newly created entity with a unique id
        Entity CreateEntity();

        // Associates component with entity
        template<typename Component>
        void AddComponent(Entity entity, Component&& component);

        // Gets component associated with entity
        template<typename Component>
        Component* GetComponent(Entity entity);

        // Returns all entities with the specified component
        template<typename Component>
        const std::vector<Entity>& GetEntitiesWithComponent();
        
    private:
        uint32_t entities = 0;

        // The entity is the key. At each entity pair, is a pair of the component type index and the actual component struct.
        std::unordered_map<Entity, std::unordered_map<std::type_index, void*>> components;

        // The component type is the key, with a vector of all entities that posseses it 
        std::unordered_map<std::type_index, std::vector<Entity>> componentIndex;
};

template<typename Component>
void EntityManager::AddComponent(Entity entity, Component&& component) {
    std::type_index componentType = typeid(Component);
    
    // Map the component to the entity
    components[entity][componentType] = new Component(std::forward<Component>(component));

    // Add component to its corrosponding pool
    componentIndex[componentType].push_back(entity);
}

template<typename Component>
const std::vector<Entity>& EntityManager::GetEntitiesWithComponent() {
    std::type_index componentType = typeid(Component);
    
    // Check if the component type exists in the index
    auto it = componentIndex.find(componentType);
    if (it != componentIndex.end()) {
        // Component type was found
        return it->second;
    }
    
    // If the component type is not found, return an empty vector
    static const std::vector<Entity> emptyVector;
    return emptyVector;
}

template<typename Component>
Component* EntityManager::GetComponent(Entity entity) {
    std::type_index componentType = typeid(Component);

    // Check if the entity exists
    auto entityIt = components.find(entity);
    if (entityIt != components.end()) {
        // Check if the component type exists for the entity
        auto componentIt = entityIt->second.find(componentType);
        if (componentIt != entityIt->second.end()) {
            // Component type found, return the component pointer
            return static_cast<Component*>(componentIt->second);
        }
    }

    // If the component or entity is not found, return nullptr
    return nullptr;
}
