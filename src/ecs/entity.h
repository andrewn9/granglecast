/*
 * entity.h
 * Each entity is given an id and is associated with components by id only
 */

#pragma once

#include <cstdint>

class Entity {
public:
    Entity(uint32_t id) : id_(id) {}

    // Return id
    uint32_t getID() const {
        return id_;
    }

private:

    // Unique identifier for each entity in the game
    uint32_t id_;
};
