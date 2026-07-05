#pragma once

/// @file Archetype.hpp
/// @brief ArchetypeId — a sorted, canonical set of ComponentIds.
///
/// Two archetypes with the same component types are IDENTICAL,
/// regardless of insertion order. Sorted vector guarantees determinism.
///
/// ArchetypeId is used as the key in World2's archetype registry.
/// Comparison is O(n) where n = component count (usually 1–16).

#include "ECS2/ComponentId.hpp"
#include <vector>
#include <algorithm>

namespace Shape::ECS2 {

/// Canonical identity of an archetype — sorted component set.
struct ArchetypeId {
    std::vector<ComponentId> components; ///< Sorted, no duplicates

    ArchetypeId() = default;

    explicit ArchetypeId(std::vector<ComponentId> ids) : components(std::move(ids)) {
        std::sort(components.begin(), components.end());
    }

    bool operator==(const ArchetypeId& o) const noexcept {
        return components == o.components;
    }
    bool operator<(const ArchetypeId& o) const noexcept {
        return components < o.components;
    }

    SHAPE_NODISCARD bool contains(ComponentId c) const noexcept {
        // Binary search (components is sorted)
        return std::binary_search(components.begin(), components.end(), c);
    }

    SHAPE_NODISCARD bool contains_all(const ArchetypeId& subset) const noexcept {
        return std::includes(components.begin(), components.end(),
                             subset.components.begin(), subset.components.end());
    }

    SHAPE_NODISCARD bool empty() const noexcept { return components.empty(); }
    SHAPE_NODISCARD usize size()  const noexcept { return components.size(); }

    /// Returns a new ArchetypeId with component C added.
    SHAPE_NODISCARD ArchetypeId with(ComponentId c) const {
        std::vector<ComponentId> v = components;
        v.push_back(c);
        std::sort(v.begin(), v.end());
        v.erase(std::unique(v.begin(), v.end()), v.end());
        return ArchetypeId{std::move(v)};
    }

    /// Returns a new ArchetypeId with component C removed.
    SHAPE_NODISCARD ArchetypeId without(ComponentId c) const {
        std::vector<ComponentId> v;
        v.reserve(components.size());
        for (ComponentId id : components) {
            if (id != c) v.push_back(id);
        }
        return ArchetypeId{std::move(v)};
    }

    /// Index of component c within sorted components array (-1 if not found).
    SHAPE_NODISCARD i32 index_of(ComponentId c) const noexcept {
        auto it = std::lower_bound(components.begin(), components.end(), c);
        if (it != components.end() && *it == c)
            return static_cast<i32>(it - components.begin());
        return -1;
    }
};

/// Empty archetype (no components) — root of the archetype graph.
inline const ArchetypeId EMPTY_ARCHETYPE{};

} // namespace Shape::ECS2
