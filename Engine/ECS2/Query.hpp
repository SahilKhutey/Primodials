#pragma once

/// @file Query.hpp + QueryCache.hpp
/// @brief ECS2 query system — iterate entities matching a component signature.
///
/// A Query<C1, C2, ...> describes "all archetypes that have AT LEAST C1 and C2".
/// The QueryCache memoizes which archetypes match each query signature.
/// On new archetype registration, the cache is invalidated.
///
/// Usage:
///   world.for_each<Transform, Velocity>([](EntityId e, Transform& t, Velocity& v) {
///       t.position += v.velocity * dt;
///   });

#include "ECS2/Archetype.hpp"
#include "ECS2/ComponentId.hpp"
#include "ECS2/ArchetypeStorage.hpp"
#include "Core/Assert.hpp"
#include <vector>
#include <functional>

namespace Shape::ECS2 {

/// Compile-time query descriptor (component type list).
template<typename... Components>
struct QueryDesc {
    static ArchetypeId make_signature() {
        std::vector<ComponentId> ids = {component_id<Components>()...};
        return ArchetypeId{std::move(ids)};
    }
};

/// Runtime query result — list of matching ArchetypeStorage pointers.
struct QueryResult {
    std::vector<ArchetypeStorage*> archetypes;

    SHAPE_NODISCARD bool empty() const noexcept { return archetypes.empty(); }
    SHAPE_NODISCARD usize size() const noexcept { return archetypes.size(); }
};

/// Cache entry: signature → matching archetypes.
class QueryCache {
public:
    /// Rebuild the cache if dirty; return the matching archetypes for sig.
    const QueryResult& get(const ArchetypeId& sig,
                           const std::vector<ArchetypeStorage*>& all_archetypes);

    /// Invalidate all cached results (call when a new archetype is created).
    void invalidate() noexcept { m_dirty = true; }

private:
    struct CacheEntry {
        ArchetypeId sig;
        QueryResult result;
    };

    void rebuild(const std::vector<ArchetypeStorage*>& all);

    std::vector<CacheEntry>          m_entries;
    std::vector<ArchetypeStorage*>   m_snapshot; ///< Last known archetype list
    bool                             m_dirty = true;
};

} // namespace Shape::ECS2
