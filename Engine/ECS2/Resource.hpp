#pragma once

/// @file Resource.hpp
/// @brief World-level singleton storage (one T per World2).
///
/// Resources are type-erased singletons stored in the World2.
/// They are NOT entities — they have no EntityId, no archetype.
///
/// Usage:
///   world.set_resource(TimeResource{.dt = 0.016f});
///   auto& time = world.resource<TimeResource>();

#include "Core/Platform.hpp"
#include "Core/Assert.hpp"
#include <any>
#include <typeindex>
#include <unordered_map>

namespace Shape::ECS2 {

class ResourceStorage {
public:
    template<typename T>
    void set(T value) {
        m_map[std::type_index(typeid(T))] = std::move(value);
    }

    template<typename T>
    T& get() {
        auto it = m_map.find(std::type_index(typeid(T)));
        SHAPE_ASSERT_MSG(it != m_map.end(), "Resource not registered");
        return std::any_cast<T&>(it->second);
    }

    template<typename T>
    const T& get() const {
        auto it = m_map.find(std::type_index(typeid(T)));
        SHAPE_ASSERT_MSG(it != m_map.end(), "Resource not registered");
        return std::any_cast<const T&>(it->second);
    }

    template<typename T>
    bool has() const {
        return m_map.count(std::type_index(typeid(T))) > 0;
    }

    template<typename T>
    T& get_or_default() {
        auto idx = std::type_index(typeid(T));
        if (m_map.find(idx) == m_map.end()) m_map[idx] = T{};
        return std::any_cast<T&>(m_map[idx]);
    }

    void clear() { m_map.clear(); }

private:
    // Note: ResourceStorage is NOT simulation-tick code; std::unordered_map is fine here.
    std::unordered_map<std::type_index, std::any> m_map;
};

} // namespace Shape::ECS2
