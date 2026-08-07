#include "ECS2/World2.hpp"
#include "ECS2/CommandBuffer2.hpp"
#include "Core/Logger.hpp"
#include "Core/Assert.hpp"
#include "ECS2/TypeInfo.hpp"
#include <cstring>

namespace Shape::ECS2 {

World2::World2() = default;

// ─── Entity management ────────────────────────────────────────────────────────

EntityId World2::create() {
    EntityId e = m_pool.alloc();
    // Entity starts with no components — no archetype placement needed yet
    // (lazy: first add() call places it)
    return e;
}

void World2::destroy(EntityId e) {
    if (!valid(e)) return;
    EntityRecord& rec = m_pool.record(e);

    if (rec.archetype_idx != ~u32{0}) {
        ArchetypeStorage* storage = m_storages[rec.archetype_idx].get();
        EntityId moved = storage->free_row(rec.chunk_idx, rec.row_index);

        // If swap-erase moved an entity, update its record
        if (moved.is_valid()) {
            EntityRecord& moved_rec = m_pool.record(moved);
            moved_rec.chunk_idx  = rec.chunk_idx;
            moved_rec.row_index  = rec.row_index;
        }
    }

    m_pool.free(e);
}

void World2::destroy_all() {
    std::vector<EntityId> to_destroy;
    for (auto& storage : m_storages) {
        for (u32 ci = 0; ci < storage->chunk_count(); ++ci) {
            Chunk* chunk = storage->chunk(ci);
            for (u32 row = 0; row < chunk->count; ++row) {
                to_destroy.push_back(chunk->entities()[row]);
            }
        }
    }
    for (auto e : to_destroy) {
        destroy(e);
    }
}

void World2::clear_all() {
    for (auto& storage : m_storages) {
        storage->clear();
    }
    m_pool.clear();
}

void World2::force_alloc_entity(EntityId e) {
    m_pool.force_alloc(e);
}

// ─── Archetype management ─────────────────────────────────────────────────────

ArchetypeStorage* World2::get_or_create_storage(const ArchetypeId& id) {
    auto it = m_archetype_map.find(id);
    if (it != m_archetype_map.end()) {
        return m_storages[it->second].get();
    }

    // New archetype
    const u32 idx = static_cast<u32>(m_storages.size());
    m_storages.push_back(std::make_unique<ArchetypeStorage>(id));
    m_archetype_ids.push_back(id);
    m_storage_ptrs.push_back(m_storages.back().get());
    m_archetype_map.emplace(id, idx);
    m_cache.invalidate(); // New archetype — cached queries may now match it
    return m_storages[idx].get();
}

void World2::move_entity(EntityId e, EntityRecord& rec, const ArchetypeId new_id) {
    const ArchetypeId old_id = current_archetype_id(rec);
    if (old_id == new_id) return;
    ArchetypeStorage* new_storage = get_or_create_storage(new_id);

    // Allocate a row in the new archetype
    auto [new_chunk, new_row] = new_storage->alloc_row(e);

    // Copy shared components from old archetype → new archetype
    if (rec.archetype_idx != ~u32{0}) {
        ArchetypeStorage* old_storage = m_storages[rec.archetype_idx].get();

        for (ComponentId cid : old_id.components) {
            if (!new_id.contains(cid)) continue; // Being removed

            const TypeInfo& ti = ComponentRegistry::get(cid);
            const usize old_off = old_storage->component_offset(cid);
            const usize new_off = new_storage->component_offset(cid);

            Chunk* old_chunk = old_storage->chunk(rec.chunk_idx);
            Chunk* n_chunk   = new_storage->chunk(new_chunk);

            void* src = old_chunk->data + old_off + rec.row_index * ti.size;
            void* dst = n_chunk->data   + new_off + new_row       * ti.size;

            // Destroy the default-constructed component in new slot, then copy
            ti.ops.destruct(dst);
            ti.ops.copy_construct(dst, src);
        }

        // Remove from old archetype
        EntityId moved = old_storage->free_row(rec.chunk_idx, rec.row_index);
        if (moved.is_valid() && moved != e) {
            EntityRecord& moved_rec = m_pool.record(moved);
            moved_rec.chunk_idx  = rec.chunk_idx;
            moved_rec.row_index  = rec.row_index;
        }
    }

    // Update this entity's record
    rec.archetype_idx = m_archetype_map.at(new_id);
    rec.chunk_idx     = new_chunk;
    rec.row_index     = new_row;
}

// ─── cmd_add_raw (CommandBuffer2 flush) ──────────────────────────────────────

void World2::cmd_add_raw(EntityId e, ComponentId cid,
                          const void* data, usize size,
                          const TypeOps& ops) {
    if (!valid(e)) return;
    EntityRecord& rec    = m_pool.record(e);
    const ArchetypeId& old_id = current_archetype_id(rec);
    const ArchetypeId  new_id = old_id.with(cid);
    move_entity(e, rec, new_id);

    // Write value
    const TypeInfo& ti = ComponentRegistry::get(cid);
    SHAPE_ASSERT_MSG(size == ti.size,
        "cmd_add_raw: size mismatch for component {} (got {}, expected {})",
        cid, size, ti.size);
    ArchetypeStorage* storage = m_storages[rec.archetype_idx].get();
    void* dst = storage->raw_array(rec.chunk_idx, cid) + rec.row_index * ti.size;
    ti.ops.destruct(dst);
    ops.copy_construct(dst, data);
}

void World2::cmd_remove_raw(EntityId e, ComponentId cid) {
    if (!valid(e)) return;
    EntityRecord& rec    = m_pool.record(e);
    const ArchetypeId  new_id = current_archetype_id(rec).without(cid);
    move_entity(e, rec, new_id);
}

// ─── Diagnostics ─────────────────────────────────────────────────────────────

usize World2::total_chunk_count() const noexcept {
    usize n = 0;
    for (const auto& s : m_storages) n += s->chunk_count();
    return n;
}

// ─── QueryCache ───────────────────────────────────────────────────────────────

const QueryResult& QueryCache::get(const ArchetypeId& sig,
                                    const std::vector<ArchetypeStorage*>& all) {
    if (m_dirty || m_snapshot != all) {
        rebuild(all);
        m_dirty    = false;
        m_snapshot = all;
    }

    for (auto& e : m_entries) {
        if (e.sig == sig) return e.result;
    }

    // New query — compute and cache
    m_entries.push_back({sig, QueryResult{}});
    QueryResult& res = m_entries.back().result;
    for (ArchetypeStorage* s : all) {
        if (s->archetype_id().contains_all(sig)) {
            res.archetypes.push_back(s);
        }
    }
    return res;
}

void QueryCache::rebuild(const std::vector<ArchetypeStorage*>& all) {
    for (auto& entry : m_entries) {
        entry.result.archetypes.clear();
        for (ArchetypeStorage* s : all) {
            if (s->archetype_id().contains_all(entry.sig)) {
                entry.result.archetypes.push_back(s);
            }
        }
    }
}

} // namespace Shape::ECS2
