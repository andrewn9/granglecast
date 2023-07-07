/*
 * eventmanager.h
 */

#pragma once

#include "events.h"
#include <unordered_map>
#include <vector>
#include <typeindex>

class EventManager {
public:
    // Maps function pointer to event type
    template<typename Event>
    void connect(void (*listener)(const Event&)) {
        listeners[typeid(Event)].push_back(reinterpret_cast<void*>(listener));
    }

    // Fires function of event
    template<typename Event>
    void fire(const Event& event) {
        auto it = listeners.find(typeid(Event));
        if (it != listeners.end()) {
            const auto& eventListeners = it->second;
            for (const auto& listener : eventListeners) {
                reinterpret_cast<void (*)(const Event&)>(listener)(event);
            }
        }
    }

private:
    // Map of event type to function pointer (listener)
    std::unordered_map<std::type_index, std::vector<void*>> listeners;
};
