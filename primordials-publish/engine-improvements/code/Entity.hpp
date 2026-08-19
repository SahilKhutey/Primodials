/**
 * Primordials — Type-Safe Generational Entity Wrapper (C++23)
 *
 * Prevents ABA recycling bugs and guarantees zero-cost indexing.
 *
 * License: MIT
 */

#pragma once

#include <cstdint>
#include <limits>
#include <vector>
#include <functional>

namespace primordials {

class Entity {
public:
    using Id = std::uint32_t;
    using Generation = std::uint32_t;

    static constexpr Id INVALID_ID = std::numeric_limits<Id>::max();
    static constexpr Generation INVALID_GEN = 0;

    constexpr Entity() noexcept = default;
    constexpr Entity(Id id, Generation gen) noexcept : id_(id), generation_(gen) {}

    [[nodiscard]] constexpr Id id() const noexcept { return id_; }
    [[nodiscard]] constexpr Generation generation() const noexcept { return generation_; }
    [[nodiscard]] constexpr bool is_valid() const noexcept { return id_ != INVALID_ID; }

    constexpr auto operator<=>(const Entity&) const noexcept = default;

private:
    Id id_{INVALID_ID};
    Generation generation_{INVALID_GEN};
};

class EntityAllocator {
public:
    using Id = Entity::Id;
    using Generation = Entity::Generation;

    EntityAllocator() noexcept = default;

    [[nodiscard]] Entity create() noexcept {
        if (!free_list_.empty()) [[unlikely]] {
            const Id id = free_list_.back();
            free_list_.pop_back();
            ++generations_[id];
            return Entity{id, generations_[id]};
        }

        if (next_id_ >= generations_.size()) [[unlikely]] {
            generations_.push_back(1);
        }

        return Entity{next_id_++, generations_[next_id_ - 1]};
    }

    void destroy(Entity e) noexcept {
        if (!is_alive(e)) [[unlikely]] return;
        free_list_.push_back(e.id());
    }

    [[nodiscard]] bool is_alive(Entity e) const noexcept {
        if (e.id() >= generations_.size()) return false;
        return e.generation() == generations_[e.id()] && e.generation() != Entity::INVALID_GEN;
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return next_id_ - free_list_.size();
    }

    void reserve(std::size_t capacity) {
        generations_.reserve(capacity);
        free_list_.reserve(capacity / 4);
    }

private:
    std::vector<Generation> generations_;
    std::vector<Id> free_list_;
    Id next_id_{0};
};

} // namespace primordials

template<>
struct std::hash<primordials::Entity> {
    std::size_t operator()(const primordials::Entity& e) const noexcept {
        const auto h1 = std::hash<primordials::Entity::Id>{}(e.id());
        const auto h2 = std::hash<primordials::Entity::Generation>{}(e.generation());
        return h1 ^ (h2 << 1);
    }
};
