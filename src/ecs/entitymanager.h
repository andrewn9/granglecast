/*
 *  entitymanager.h 
 *  Holds the EntityManager class, which manages entities and their components.
 *  Contains methods for the creation, addition, retrieval, and removal of components associated with entities.
 */

#pragma once

#include <unordered_map>
#include <bitset>
#include <typeindex>
#include <queue>
#include <cassert>
#include <any>

#include "components.h"
#include "../util/sset.h"

#define MAX_ENTITIES    500
#define MAX_COMPONENTS  5

typedef uint32_t Entity;

class EntityManager {
    public:
        Entity CreateEntity();
        void RemoveEntity(Entity entity);
        void PrintData() const;

        // Add a component to an entity
        template<typename Component>
        void AddComponent(Entity entity, const Component& component) {
            // If the entity is 0, return
            if (entity == 0) {
                return;
            }

            // Get the type of the component
            std::type_index component_type = typeid(Component);

            // If the entity already has the component, return
            if (component_data.count(entity) > 0 && component_data[entity].count(component_type) > 0) {
                return;
            } else {
                // Add the component to the entity
                component_data[entity][component_type] = component;
                // Add the entity to the component set
                component_sets[component_type].insert(entity);
            }
        }

        // Remove a component from an entity
        template<typename Component>
        void RemoveComponent(Entity entity) {
            // If entity is 0, return
            if (entity == 0) {
                return;
            }

            // Get the type of component
            std::type_index component_type = typeid(Component);

            // If the entity has the component and the component type is in the data
            if (component_data.count(entity) > 0 && component_data[entity].count(component_type) > 0) {
                // Remove the component from the data
                component_data[entity].erase(component_type);
                // Remove the entity from the component set
                component_sets[component_type].erase(entity);
            }
        }

        // Get a component from an entity
        template<typename Component>
        Component* GetComponent(Entity entity) {
            // Get the type of component
            std::type_index component_type = typeid(Component);

            // If the entity has the component and the component type is in the data
            if (component_data.count(entity) > 0 && component_data[entity].count(component_type) > 0) {
                // Return the component
                return &std::any_cast<Component&>(component_data[entity][component_type]);
            }

            // Return nullptr if the entity does not have the component
            return nullptr;
        }
        
        // Checks presence of component
        template<typename Component>
        bool HasComponent(Entity entity) const {
            std::type_index component_type = typeid(Component);
            auto it = component_sets.find(component_type);
            return (it != component_sets.end()) && it->second.contains(entity);
        }

        template<typename Component>
        SparseSet<Entity>& GetEntitiesWithComponent() {
            std::type_index component_type = typeid(Component);
            return component_sets[component_type];
        }
        

    private:
        // The next entity to be created
        Entity next_entity = 1;

        // Queue of last deleted entities
        std::queue<Entity> inactive_entities;

        // Stores component maps for each entity
        std::unordered_map<Entity, std::unordered_map<std::type_index, std::any>> component_data;

        // Sparse sets of each component type
        std::unordered_map<std::type_index, SparseSet<Entity>> component_sets;
};

