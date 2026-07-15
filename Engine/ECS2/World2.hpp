#pragma once

/// @file World2.hpp
/// @brief Archetype-based ECS2 World — the main API for Phase B and beyond.
///
/// Key capabilities:
///   - Create/destroy entities (O(1) amortized)
///   - Add/remove components (entity moves between archetypes, O(1) amortized)
///   - for_each<C1,C2,...>() — cache-friendly chunk iteration, zero allocation
///   - Resources — world-level singletons
///   - CommandBuffer2 — deferred structural changes during iteration
///
/// DETERMINISM:
///   - Entity creation order is deterministic (free-list, then monotone)
///   - Archetype registration order depends on first-use order (fix via explicit register)
///   - for_each iterates archetypes in sorted ArchetypeId order (deterministic)
///   - Do NOT use resource<T>() as a substitute for DeterministicRng

#include "ECS2/EntityId.hpp"
#include "ECS2/EntityPool.hpp"
#include "ECS2/Archetype.hpp"
#include "ECS2/ArchetypeStorage.hpp"
#include "ECS2/ComponentRegistry.hpp"
#include "ECS2/Query.hpp"
#include "ECS2/Resource.hpp"
#include "Core/Assert.hpp"
#include <map>
#include <vector>
#include <memory>

namespace Shape::ECS2 {

class CommandBuffer2; // forward

class World2 {
public:
    World2();
    ~World2() = default;

    World2(const World2&) = delete;
    World2& operator=(const World2&) = delete;

    // ── Entity management ─────────────────────────────────────────────────────

    /// Create a new entity (no components, placed in empty archetype).
    SHAPE_NODISCARD EntityId create();

    /// Destroy an entity and all its components.
    void destroy(EntityId e);
    void destroy_all();
    void clear_all();
    void force_alloc_entity(EntityId e);

    /// Is this handle still valid?
    SHAPE_NODISCARD bool valid(EntityId e) const noexcept { return m_pool.valid(e); }

    SHAPE_NODISCARD usize entity_count() const noexcept { return m_pool.alive_count(); }

    // ── Component add/remove ──────────────────────────────────────────────────

    /// Add component T to entity e (value-initialized by default).
    template<typename T>
    T& add(EntityId e, T value = T{}) {
        SHAPE_ASSERT(valid(e));
        const ComponentId cid = component_id<T>();

        EntityRecord& rec = m_pool.record(e);
        const ArchetypeId& old_id = current_archetype_id(rec);
        const ArchetypeId  new_id = old_id.with(cid);

        // Move entity to new archetype
        move_entity(e, rec, new_id);

        // Write the component value
        T* slot = storage_for(rec)->template get<T>(rec.chunk_idx, rec.row_index);
        *slot = std::move(value);
        return *slot;
    }

    /// Remove component T from entity e.
    template<typename T>
    void remove(EntityId e) {
        SHAPE_ASSERT(valid(e));
        const ComponentId cid = component_id<T>();
        EntityRecord& rec = m_pool.record(e);
        const ArchetypeId new_id = current_archetype_id(rec).without(cid);
        move_entity(e, rec, new_id);
    }

    /// Does entity e have component T?
    template<typename T>
    SHAPE_NODISCARD bool has(EntityId e) const noexcept {
        if (!valid(e)) return false;
        const EntityRecord& rec = m_pool.record(e);
        if (rec.archetype_idx == ~u32{0}) return false;
        return m_archetype_ids[rec.archetype_idx].contains(component_id<T>());
    }

    /// Get a reference to component T on entity e. Asserts if not present.
    template<typename T>
    SHAPE_NODISCARD T& get(EntityId e) {
        SHAPE_ASSERT(has<T>(e));
        EntityRecord& rec = m_pool.record(e);
        return *storage_for(rec)->template get<T>(rec.chunk_idx, rec.row_index);
    }

    template<typename T>
    SHAPE_NODISCARD const T& get(EntityId e) const {
        return const_cast<World2*>(this)->get<T>(e);
    }

    // ── for_each ──────────────────────────────────────────────────────────────

    /// Iterate all entities that have AT LEAST {Components...}.
    /// Fn signature: void(Components&...)  OR  void(EntityId, Components&...)
    template<typename... Components, typename Fn>
    void for_each(Fn&& fn) {
        const ArchetypeId sig = QueryDesc<Components...>::make_signature();
        const QueryResult& result = m_cache.get(sig, m_storage_ptrs);

        for (ArchetypeStorage* storage : result.archetypes) {
            const usize n_chunks = storage->chunk_count();
            for (u32 ci = 0; ci < static_cast<u32>(n_chunks); ++ci) {
                Chunk* chunk = storage->chunk(ci);
                const u32 count = chunk->count;
                for (u32 row = 0; row < count; ++row) {
                    dispatch_fn<Components...>(fn, storage, chunk, ci, row);
                }
            }
        }
    }

    // ── Resources ─────────────────────────────────────────────────────────────

    template<typename T>
    T& resource() { return m_resources.get_or_default<T>(); }

    template<typename T>
    const T& resource() const { return m_resources.get<T>(); }

    template<typename T>
    void set_resource(T value) { m_resources.set(std::move(value)); }

    // ── Diagnostics ───────────────────────────────────────────────────────────

    SHAPE_NODISCARD usize archetype_count()   const noexcept { return m_storages.size(); }
    SHAPE_NODISCARD usize total_chunk_count() const noexcept;

    // ── Internal (used by CommandBuffer2) ─────────────────────────────────────

    void cmd_destroy(EntityId e)                           { destroy(e); }
    void cmd_add_raw(EntityId e, ComponentId cid,
                     const void* data, usize size,
                     const TypeOps& ops);
    void cmd_remove_raw(EntityId e, ComponentId cid);

private:
    ArchetypeStorage* get_or_create_storage(const ArchetypeId& id);
    void move_entity(EntityId e, EntityRecord& rec, const ArchetypeId new_id);

    SHAPE_NODISCARD ArchetypeStorage* storage_for(const EntityRecord& rec) noexcept {
        if (rec.archetype_idx == ~u32{0}) return nullptr;
        return m_storages[rec.archetype_idx].get();
    }

    SHAPE_NODISCARD ArchetypeId current_archetype_id(const EntityRecord& rec) const noexcept {
        if (rec.archetype_idx == ~u32{0}) return EMPTY_ARCHETYPE;
        return m_archetype_ids[rec.archetype_idx];
    }

    /// Type-safe dispatcher: calls fn(EntityId, C1&, C2&, ...) OR fn(C1&, C2&, ...)
    template<typename... Components, typename Fn>
    static void dispatch_fn(Fn&& fn, ArchetypeStorage* storage,
                            Chunk* chunk, u32 ci, u32 row) {
        if constexpr (std::is_invocable_v<Fn, EntityId, Components&...>) {
            fn(chunk->entities()[row], (*storage->template get<Components>(ci, row))...);
        } else {
            fn((*storage->template get<Components>(ci, row))...);
        }
    }

    EntityPool                                   m_pool;
    // Archetypes stored in sorted map for deterministic iteration
    std::map<ArchetypeId, u32>                   m_archetype_map;  ///< ArchetypeId → index
    std::vector<std::unique_ptr<ArchetypeStorage>> m_storages;
    std::vector<ArchetypeId>                     m_archetype_ids;
    std::vector<ArchetypeStorage*>               m_storage_ptrs;   ///< Parallel to m_storages
    QueryCache                                   m_cache;
    ResourceStorage                              m_resources;
};

} // namespace Shape::ECS2

