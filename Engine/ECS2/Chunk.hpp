#pragma once

/// @file Chunk.hpp
/// @brief 16 KB aligned chunk storing entities of a single archetype (SoA layout).
///
/// Layout inside a chunk:
///   [EntityId * capacity][padding][C0 * capacity][padding][C1 * capacity]...
///
/// capacity is computed by ArchetypeStorage based on component sizes.
/// All component arrays are aligned to their respective alignof(T).
///
/// Chunks are allocated and owned by ArchetypeStorage.
/// Entity→chunk mapping is maintained by EntityPool records.

#include "ECS2/EntityId.hpp"
#include "ECS2/ComponentId.hpp"
#include "Core/Assert.hpp"
#include <cstddef>
#include <cstring>

namespace Shape::ECS2 {

/// How one component type is laid out inside a Chunk.
struct ComponentOffset {
    ComponentId id;     ///< Component type
    usize       offset; ///< Byte offset from chunk data[0]
    usize       size;   ///< sizeof(T)
};

/// 16 KB aligned chunk buffer. Managed by ArchetypeStorage.
struct alignas(64) Chunk {
    static constexpr usize CHUNK_SIZE = 16 * 1024;
    static constexpr usize HEADER     = 64;
    static constexpr usize DATA_SIZE  = CHUNK_SIZE - HEADER;

    // ── Header (64 bytes) ─────────────────────────────────────────────────────
    u32 count    = 0; ///< Current entity count
    u32 capacity = 0; ///< Max entities this chunk can hold

    u8 _pad[56]{}; // Pad to 64 bytes

    // ── Data ──────────────────────────────────────────────────────────────────
    alignas(64) u8 data[DATA_SIZE]{};

    // ── Entity array (always at offset 0) ─────────────────────────────────────

    EntityId* entities() noexcept {
        return reinterpret_cast<EntityId*>(data);
    }
    const EntityId* entities() const noexcept {
        return reinterpret_cast<const EntityId*>(data);
    }

    // ── Component access ──────────────────────────────────────────────────────

    u8* component_data(usize byte_offset) noexcept {
        return data + byte_offset;
    }
    const u8* component_data(usize byte_offset) const noexcept {
        return data + byte_offset;
    }

    // ── State ─────────────────────────────────────────────────────────────────

    SHAPE_NODISCARD bool full()  const noexcept { return count >= capacity; }
    SHAPE_NODISCARD bool empty() const noexcept { return count == 0; }

    /// Return the row index where the next entity will be placed.
    SHAPE_NODISCARD u32 next_row() const noexcept {
        SHAPE_ASSERT(!full());
        return count;
    }
};

static_assert(sizeof(Chunk) == Chunk::CHUNK_SIZE, "Chunk must be exactly CHUNK_SIZE bytes");
static_assert(alignof(Chunk) == 64, "Chunk must be 64-byte aligned");

} // namespace Shape::ECS2
