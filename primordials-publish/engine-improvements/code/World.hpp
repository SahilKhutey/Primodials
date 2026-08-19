/**
 * Primordials — Archetype-based Structure-of-Arrays (SoA) ECS (C++23)
 *
 * High-performance, cache-aligned entity component system.
 *
 * License: MIT
 */

#pragma once

#include "Entity.hpp"

#include <algorithm>
#include <concepts>
#include <memory_resource>
#include <optional>
#include <span>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace primordials {

template<typename T>
concept Component = std::is_trivially_copyable_v<T> && std::is_default_constructible_v<T>;

namespace detail {
    inline std::uint32_t& component_counter() noexcept {
        static std::uint32_t counter = 0;
        return counter;
    }

    template<Component T>
    inline const std::uint32_t component_id_v = []() -> std::uint32_t {
        return component_counter()++;
    }();
} // namespace detail

template<Component T>
[[nodiscard]] constexpr std::uint32_t component_id() noexcept {
    return detail::component_id_v<T>;
}

class World {
public:
    explicit World(std::pmr::memory_resource* alloc = std::pmr::get_default_resource())
        : allocator_(alloc)
        , entity_allocator_() {}

    template<Component T>
    void register_component() {
        component_pools_[component_id<T>()] = {};
    }

    template<Component T>
    [[nodiscard]] T* get() noexcept {
        const auto id = component_id<T>();
        auto it = component_pools_.find(id);
        if (it == component_pools_.end() || it->second.empty()) return nullptr;
        return reinterpret_cast<T*>(it->second.data());
    }

    template<Component T>
    [[nodiscard]] T& get(Entity e) {
        auto* pool = get<T>();
        return pool[e.id()];
    }

    template<Component T, typename... Args>
    Entity create_entity(Args&&... args) {
        Entity e = entity_allocator_.create();
        const auto id = component_id<T>();
        auto& pool = component_pools_[id];
        if (pool.size() <= e.id() * sizeof(T)) {
            pool.resize((e.id() + 1) * sizeof(T));
        }
        new (reinterpret_cast<T*>(pool.data()) + e.id()) T{std::forward<Args>(args)...};
        return e;
    }

    void destroy(Entity e) noexcept {
        entity_allocator_.destroy(e);
    }

    [[nodiscard]] std::size_t entity_count() const noexcept {
        return entity_allocator_.size();
    }

private:
    std::pmr::memory_resource* allocator_;
    EntityAllocator entity_allocator_;
    std::unordered_map<std::uint32_t, std::vector<std::byte>> component_pools_;
};

} // namespace primordials
