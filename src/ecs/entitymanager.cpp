/*
 *  entitymanager.cpp 
 *  Contains methods for the creation and removal of entities.
 */

#include "entitymanager.h"
#include "world.h"
#include <string>

Entity EntityManager::CreateEntity() {
    Entity entity;
    if (!inactive_entities.empty()) {
        entity = inactive_entities.front();
        inactive_entities.pop();
    } else {
        if (components.size() >= MAX_ENTITIES) {
            return 0;
        }

        entity = static_cast<Entity>(components.size());
        components.emplace_back();
    }
    entity_mask.set(entity);
    return entity;
}

void EntityManager::RemoveEntity(Entity entity) {
    if (entity == 0 || entity >= MAX_ENTITIES) {
        return;
    }

    entity_mask.reset(entity);
    components[entity].clear();
    inactive_entities.push(entity);

    for (auto& indexPair : componentIndex) {
        auto& entities = indexPair.second;
        entities.erase(std::remove_if(entities.begin(), entities.end(),
            [entity](Entity e) { return e == entity; }), entities.end());
    }
}

void EntityManager::PrintData() const {
    for (size_t entity = 1; entity < components.size(); ++entity) {
        if (entity_mask.test(entity)) {
            for (const auto& componentPair : components[entity]) {
                std::type_index component_type = componentPair.first;
                void* component = componentPair.second;
                SDL_Log("Entity: %u | Type: %s | Component: %p", entity, component_type.name(), component);
            }
        }
    }

    for (const auto& indexPair : componentIndex) {
        std::type_index component_type = indexPair.first;
        const auto& entities = indexPair.second;

        std::string entitiesString;
        for (const auto& entity : entities) {
            entitiesString += std::to_string(entity) + ", ";
        }

        if (!entitiesString.empty()) {
            entitiesString = entitiesString.substr(0, entitiesString.length() - 2);
        }

        SDL_Log("Type: %s | Entities: %s", component_type.name(), entitiesString.c_str());
    }
}