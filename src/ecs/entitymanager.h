/*
 *  entitymanager.h 
 *  Holds the EntityManager class, which manages entities and their components.
 *  Contains methods for the creation, addition, retrieval, and removal of components associated with entities.
 */

#pragma once

#include <unordered_map>
#include <vector>
#include <bitset>
#include <typeindex>
#include <queue>
#include <algorithm>
#include <string>

#include "components.h"

typedef uint32_t Entity;

class EntityManager {
public:
    Entity CreateEntity();
    void RemoveEntity(Entity entity);

    template<typename Component>
    void AddComponent(Entity entity, Component&& component);

    template<typename Component>
    void RemoveComponent(Entity entity);

    template<typename Component>
    Component* GetComponent(Entity entity) const;

    template<typename Component>
    bool HasComponent(Entity entity) const;

    template<typename Component>
    const std::vector<Entity>& GetEntitiesWithComponent() const;

    void PrintData() const;

private:
    static constexpr size_t MAX_ENTITIES = 10000;

    std::vector<std::unordered_map<std::type_index, void*>> components;
    std::unordered_map<std::type_index, std::vector<Entity>> componentIndex;
    std::queue<Entity> inactive_entities;
    std::bitset<MAX_ENTITIES> entity_mask;
};

template<typename Component>
void EntityManager::AddComponent(Entity entity, Component&& component) {
    if (entity == 0 || entity >= MAX_ENTITIES) {
        return;
    }

    std::type_index component_type = typeid(Component);

    components[entity][component_type] = new Component(std::forward<Component>(component));
    componentIndex[component_type].push_back(entity);
}

template<typename Component>
void EntityManager::RemoveComponent(Entity entity) {
    if (entity == 0 || entity >= MAX_ENTITIES) {
        return;
    }

    std::type_index component_type = typeid(Component);

    auto entity_iterator = components[entity].find(component_type);
    if (entity_iterator != components[entity].end()) {
        delete static_cast<Component*>(entity_iterator->second);
        components[entity].erase(entity_iterator);

        auto index_iterator = componentIndex.find(component_type);
        if (index_iterator != componentIndex.end()) {
            auto& entities = index_iterator->second;
            entities.erase(std::remove_if(entities.begin(), entities.end(),
                [entity](Entity e) { return e == entity; }), entities.end());
        }
    }
}

template<typename Component>
Component* EntityManager::GetComponent(Entity entity) const {
    if (entity >= MAX_ENTITIES) {
        return nullptr;
    }

    std::type_index component_type = typeid(Component);

    if (entity_mask.test(entity)) {
        auto entity_iterator = components[entity].find(component_type);
        if (entity_iterator != components[entity].end()) {
            return static_cast<Component*>(entity_iterator->second);
        }
    }

    return nullptr;
}

template<typename Component>
bool EntityManager::HasComponent(Entity entity) const {
    if (entity == 0 || entity >= MAX_ENTITIES) {
        return false;
    }

    std::type_index componentType = typeid(Component);

    if (components.size() <= entity) {
        return false;
    }

    const auto& entityComponents = components[entity];
    return entityComponents.find(componentType) != entityComponents.end();
}

template<typename Component>
const std::vector<Entity>& EntityManager::GetEntitiesWithComponent() const {
    std::type_index component_type = typeid(Component);
    
    auto it = componentIndex.find(component_type);
    if (it != componentIndex.end()) {
        return it->second;
    }
    
    static const std::vector<Entity> empty;
    return empty;
}

