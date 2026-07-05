#include "ECS2/ArchetypeStorage.hpp"
#include "Core/Logger.hpp"
#include <cstring>
#include <new>

namespace Shape::ECS2 {

// ─── Layout computation ───────────────────────────────────────────────────────

static usize align_up(usize n, usize align) noexcept {
    return (n + align - 1) & ~(align - 1);
}

void ArchetypeStorage::compute_layout() {
    if (m_id.empty()) {
        m_chunk_capacity = 0;
        return;
    }

    // Step 1: compute row_size = sizeof(EntityId) + sum of component sizes
    // (we need to account for alignment padding between arrays)
    // We'll use a two-pass approach:
    //   Pass 1: find total aligned row size to compute capacity
    //   Pass 2: assign offsets based on capacity

    // Entity IDs come first
    usize entity_array_size = sizeof(EntityId); // per entity

    // Per-entity component sizes
    usize per_entity_size = sizeof(EntityId);
    for (ComponentId cid : m_id.components) {
        const TypeInfo& ti = ComponentRegistry::get(cid);
        per_entity_size += ti.size;
    }

    // Capacity = floor(DATA_SIZE / per_entity_size) with some conservative margin
    m_chunk_capacity = static_cast<u32>(Chunk::DATA_SIZE / per_entity_size);
    if (m_chunk_capacity == 0) m_chunk_capacity = 1;

    // Step 2: assign offsets for SoA layout
    // [EntityId * cap][C0 * cap][C1 * cap] ...  (aligned per type)
    m_offsets.clear();
    usize offset = 0;

    // Entity IDs (always first)
    offset = align_up(offset, alignof(EntityId));
    offset += sizeof(EntityId) * m_chunk_capacity;

    for (ComponentId cid : m_id.components) {
        const TypeInfo& ti = ComponentRegistry::get(cid);
        offset = align_up(offset, ti.alignment);

        if (offset + ti.size * m_chunk_capacity > Chunk::DATA_SIZE) {
            // This component doesn't fit; reduce capacity
            while (m_chunk_capacity > 1 &&
                   offset + ti.size * m_chunk_capacity > Chunk::DATA_SIZE) {
                --m_chunk_capacity;
            }
        }

        m_offsets.push_back(ComponentOffset{cid, offset, ti.size});
        offset += ti.size * m_chunk_capacity;
    }
}

// ─── Constructor ──────────────────────────────────────────────────────────────

ArchetypeStorage::ArchetypeStorage(const ArchetypeId& id) : m_id(id) {
    compute_layout();
}

ArchetypeStorage::~ArchetypeStorage() = default;

// ─── Chunk allocation ─────────────────────────────────────────────────────────

Chunk* ArchetypeStorage::alloc_chunk() {
    // Allocate aligned 16KB chunk
    void* mem = ::operator new[](sizeof(Chunk), std::align_val_t{64});
    Chunk* chunk = new (mem) Chunk{};
    chunk->capacity = m_chunk_capacity;
    chunk->count    = 0;

    // Default-construct all component arrays? No — we construct per-entity on alloc.
    m_chunks.push_back(std::unique_ptr<Chunk>(chunk));
    return chunk;
}

// ─── alloc_row ────────────────────────────────────────────────────────────────

ArchetypeStorage::AllocResult ArchetypeStorage::alloc_row(EntityId e) {
    // Find a chunk with free space
    u32 chunk_idx = ~u32{0};
    for (u32 i = 0; i < static_cast<u32>(m_chunks.size()); ++i) {
        if (!m_chunks[i]->full()) { chunk_idx = i; break; }
    }
    if (chunk_idx == ~u32{0}) {
        alloc_chunk();
        chunk_idx = static_cast<u32>(m_chunks.size()) - 1;
    }

    Chunk* c   = m_chunks[chunk_idx].get();
    u32    row = c->count;

    // Write entity ID
    c->entities()[row] = e;

    // Default-construct all components at this row
    for (const auto& co : m_offsets) {
        const TypeInfo& ti = ComponentRegistry::get(co.id);
        void* dst = c->data + co.offset + row * co.size;
        ti.ops.default_construct(dst);
    }

    ++c->count;
    ++m_entity_count;
    return {chunk_idx, row};
}

// ─── free_row ─────────────────────────────────────────────────────────────────

EntityId ArchetypeStorage::free_row(u32 chunk_idx, u32 row) {
    SHAPE_ASSERT(chunk_idx < m_chunks.size());
    Chunk* c = m_chunks[chunk_idx].get();
    SHAPE_ASSERT(row < c->count);

    const u32 last = c->count - 1;
    EntityId moved_entity = EntityId::invalid();

    if (row != last) {
        // Swap-erase: move last row into this row
        moved_entity = c->entities()[last];
        c->entities()[row] = moved_entity;

        for (const auto& co : m_offsets) {
            const TypeInfo& ti = ComponentRegistry::get(co.id);
            u8* dst = c->data + co.offset + row  * co.size;
            u8* src = c->data + co.offset + last * co.size;
            // Destroy dst, move src into dst, destroy src
            ti.ops.destruct(dst);
            ti.ops.move_construct(dst, src);
        }
    } else {
        // Last element — just destroy
        for (const auto& co : m_offsets) {
            const TypeInfo& ti = ComponentRegistry::get(co.id);
            void* ptr = c->data + co.offset + row * co.size;
            ti.ops.destruct(ptr);
        }
    }

    --c->count;
    --m_entity_count;
    return moved_entity;
}

} // namespace Shape::ECS2
