#pragma once

/// @file ArchetypeStorage.hpp
/// @brief Manages the set of Chunks for one Archetype.
///
/// ArchetypeStorage:
///   - Computes chunk layout (capacity, per-component offsets) from TypeInfos
///   - Allocates Chunks on demand (new, aligned)
///   - Provides O(1) row insert / swap-erase
///   - Exposes typed get<T>(row) for fast component access during query iteration

#include "ECS2/Archetype.hpp"
#include "ECS2/Chunk.hpp"
#include "ECS2/ComponentRegistry.hpp"
#include "Core/Assert.hpp"
#include <vector>
#include <memory>

namespace Shape::ECS2 {

class ArchetypeStorage {
public:
    explicit ArchetypeStorage(const ArchetypeId& id);
    ~ArchetypeStorage();

    ArchetypeStorage(const ArchetypeStorage&) = delete;
    ArchetypeStorage& operator=(const ArchetypeStorage&) = delete;

    // ── Layout info ───────────────────────────────────────────────────────────

    SHAPE_NODISCARD const ArchetypeId& archetype_id() const noexcept { return m_id; }
    SHAPE_NODISCARD u32 chunk_capacity()              const noexcept { return m_chunk_capacity; }
    SHAPE_NODISCARD usize chunk_count()               const noexcept { return m_chunks.size(); }
    SHAPE_NODISCARD usize entity_count()              const noexcept { return m_entity_count; }

    // ── Allocation ────────────────────────────────────────────────────────────

    /// Allocate a new row. Returns {chunk_idx, row_in_chunk}.
    /// Grows chunk list if needed.
    struct AllocResult { u32 chunk_idx; u32 row; };
    AllocResult alloc_row(EntityId e);

    /// Remove the entity at {chunk_idx, row}. Swap-erases (copies last element over).
    /// Returns the EntityId that was moved (so the caller can update its record), or
    /// EntityId::invalid() if the deleted entity was already the last.
    EntityId free_row(u32 chunk_idx, u32 row);

    /// Clear all rows and destruct components.
    void clear() noexcept;

    // ── Component access ──────────────────────────────────────────────────────

    /// Get a typed pointer to component c in chunk chunk_idx at row.
    template<typename T>
    T* get(u32 chunk_idx, u32 row) noexcept {
        SHAPE_ASSERT(chunk_idx < m_chunks.size());
        const ComponentId cid = component_id<T>();
        const usize off = component_offset(cid);
        SHAPE_ASSERT(off != ~usize{0});
        Chunk* chunk = m_chunks[chunk_idx].get();
        SHAPE_ASSERT(row < chunk->count);
        return reinterpret_cast<T*>(chunk->data + off + row * sizeof(T));
    }

    template<typename T>
    const T* get(u32 chunk_idx, u32 row) const noexcept {
        return const_cast<ArchetypeStorage*>(this)->get<T>(chunk_idx, row);
    }

    /// Get raw pointer to component array (for a single chunk).
    u8* raw_array(u32 chunk_idx, ComponentId cid) noexcept {
        SHAPE_ASSERT(chunk_idx < m_chunks.size());
        const usize off = component_offset(cid);
        SHAPE_ASSERT(off != ~usize{0});
        return m_chunks[chunk_idx]->data + off;
    }

    // ── Chunk access (for query iteration) ────────────────────────────────────

    SHAPE_NODISCARD Chunk* chunk(u32 idx) noexcept {
        SHAPE_ASSERT(idx < m_chunks.size());
        return m_chunks[idx].get();
    }
    SHAPE_NODISCARD const Chunk* chunk(u32 idx) const noexcept {
        return const_cast<ArchetypeStorage*>(this)->chunk(idx);
    }

    // ── Component offset lookup ───────────────────────────────────────────────

    SHAPE_NODISCARD usize component_offset(ComponentId id) const noexcept {
        for (const auto& co : m_offsets) {
            if (co.id == id) return co.offset;
        }
        return ~usize{0}; // not found
    }

private:
    void compute_layout();
    Chunk* alloc_chunk();

    ArchetypeId                        m_id;
    std::vector<ComponentOffset>       m_offsets;      ///< Per-component byte offsets in chunk
    std::vector<std::unique_ptr<Chunk>> m_chunks;      ///< Heap-allocated 16KB chunks
    u32                                m_chunk_capacity = 0; ///< Entities per chunk
    usize                              m_entity_count   = 0;
};

} // namespace Shape::ECS2
