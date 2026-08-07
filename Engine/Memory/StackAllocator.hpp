#pragma once

#include "Memory/Allocator.hpp"
#include "Core/Assert.hpp"

namespace Shape {

// ─────────────────────────────────────────────────────────────────────────────
//  StackAllocator — LIFO bump pointer with marker-based rollback.
//
//  Rules:
//    - Allocations grow upward.
//    - SaveMarker() / RestoreMarker(m) provide scoped allocation regions.
//    - Deallocate(ptr) is only valid for the most recent live block
//      (asserts in debug, no-ops in release to remain safe).
// ─────────────────────────────────────────────────────────────────────────────
class StackAllocator final : public Allocator {
public:
    using Marker = usize;

    explicit StackAllocator(usize capacity)
        : m_Buffer(new u8[capacity])
        , m_Capacity(capacity)
        , m_Offset(0) {}

    ~StackAllocator() override = default;

    void* Allocate(usize size, usize alignment = alignof(std::max_align_t)) override {
        // Store the previous offset just before the allocation so we can free it
        usize headerSize = sizeof(Marker);
        usize base   = reinterpret_cast<usize>(m_Buffer.get() + m_Offset) + headerSize;
        usize aligned = AlignAddress(base, alignment);
        usize prevOff = aligned - headerSize - reinterpret_cast<usize>(m_Buffer.get());
        usize totalEnd = aligned - reinterpret_cast<usize>(m_Buffer.get()) + size;

        SHAPE_ASSERT_MSG(totalEnd <= m_Capacity,
            "StackAllocator overflow: need {}, capacity {}", totalEnd, m_Capacity);

        // Write previous offset header just before the aligned block
        *reinterpret_cast<Marker*>(m_Buffer.get() + prevOff) = m_Offset;
        m_Offset = totalEnd;

        return reinterpret_cast<void*>(aligned);
    }

    /// Free the most recently allocated block.
    void Deallocate(void* ptr) override {
        if (!ptr) return;
        // Read the saved offset stored in the header just before ptr
        Marker* header = reinterpret_cast<Marker*>(
            reinterpret_cast<u8*>(ptr) - sizeof(Marker));
        SHAPE_ASSERT_MSG(
            reinterpret_cast<u8*>(ptr) >= m_Buffer.get() &&
            reinterpret_cast<u8*>(ptr) <  m_Buffer.get() + m_Capacity,
            "StackAllocator::Deallocate: pointer out of range");
        m_Offset = *header;
    }

    /// Save the current top-of-stack marker.
    Marker SaveMarker() const { return m_Offset; }

    /// Roll back to a previously saved marker (frees everything above it).
    void RestoreMarker(Marker marker) {
        SHAPE_ASSERT_MSG(marker <= m_Offset, "StackAllocator::RestoreMarker: marker is above current top");
        m_Offset = marker;
    }

    /// Full reset
    void Reset() override { m_Offset = 0; }

    usize Used()     const { return m_Offset; }
    usize Capacity() const { return m_Capacity; }

private:
    std::unique_ptr<u8[]> m_Buffer;
    usize m_Capacity;
    usize m_Offset;
};

} // namespace Shape
