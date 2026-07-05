#pragma once

/// @file EntityPool.hpp
/// @brief Generation-checked entity handle pool.
///
/// Manages a free-list of entity slots. Each slot has:
///   - generation: incremented on each deallocation so stale handles fail validation
///   - archetype_index: which archetype this entity currently belongs to (-1 = no archetype)
///   - row_index: which row within the archetype's chunk storage
///
/// Capacity doubles on demand (heap-backed, not arena — entity pools are long-lived).

#include "ECS2/EntityId.hpp"
#include "Core/Assert.hpp"
#include <vector>

namespace Shape::ECS2 {

/// Internal record stored per entity slot.
struct EntityRecord {
    u32 generation    = 0;
    u32 archetype_idx = ~u32{0}; ///< Index into World2::m_archetypes (~0 = not placed)
    u32 chunk_idx     = ~u32{0}; ///< Which chunk within the archetype storage
    u32 row_index     = ~u32{0}; ///< Row within the chunk
};

class EntityPool {
public:
    EntityPool() = default;

    /// Allocate a new entity handle.
    SHAPE_NODISCARD EntityId alloc() {
        if (!m_free_list.empty()) {
            const u32 idx = m_free_list.back();
            m_free_list.pop_back();
            return EntityId{idx, m_records[idx].generation};
        }
        const u32 idx = static_cast<u32>(m_records.size());
        m_records.push_back(EntityRecord{0, ~u32{0}, ~u32{0}});
        return EntityId{idx, 0};
    }

    /// Free an entity handle. Increments generation to invalidate stale handles.
    void free(EntityId e) noexcept {
        SHAPE_ASSERT(valid(e));
        auto& rec = m_records[e.index];
        ++rec.generation;
        rec.archetype_idx = ~u32{0};
        rec.row_index     = ~u32{0};
        m_free_list.push_back(e.index);
    }

    /// Is this handle still valid?
    SHAPE_NODISCARD bool valid(EntityId e) const noexcept {
        return e.index < m_records.size() &&
               m_records[e.index].generation == e.generation;
    }

    /// Access the EntityRecord for a valid handle.
    SHAPE_NODISCARD EntityRecord& record(EntityId e) noexcept {
        SHAPE_ASSERT(valid(e));
        return m_records[e.index];
    }
    SHAPE_NODISCARD const EntityRecord& record(EntityId e) const noexcept {
        SHAPE_ASSERT(valid(e));
        return m_records[e.index];
    }

    SHAPE_NODISCARD usize alive_count() const noexcept {
        return m_records.size() - m_free_list.size();
    }

    SHAPE_NODISCARD usize capacity() const noexcept { return m_records.size(); }

private:
    std::vector<EntityRecord> m_records;
    std::vector<u32>          m_free_list;
};

} // namespace Shape::ECS2
