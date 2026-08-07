#pragma once

#include "Memory/Allocator.hpp"
#include "Core/Assert.hpp"

namespace Shape {

// ─────────────────────────────────────────────────────────────────────────────
//  FrameAllocator — reset every tick/frame, no individual frees.
//
//  Policy:
//    - Backed by a single contiguous buffer (owned or externally supplied).
//    - Bump-pointer allocation with alignment support.
//    - Reset() rewinds the pointer; no destructors called (use for POD).
//    - Thread-unsafe by design — one per thread / one per simulation tick.
// ─────────────────────────────────────────────────────────────────────────────
class FrameAllocator final : public Allocator {
public:
    /// Allocate and own a buffer of `capacity` bytes.
    explicit FrameAllocator(usize capacity)
        : m_OwnedBuffer(new u8[capacity])
        , m_Buffer(m_OwnedBuffer.get())
        , m_Capacity(capacity)
        , m_Offset(0) {}

    /// Wrap an externally owned buffer (no ownership transfer).
    FrameAllocator(void* buffer, usize capacity)
        : m_OwnedBuffer(nullptr)
        , m_Buffer(static_cast<u8*>(buffer))
        , m_Capacity(capacity)
        , m_Offset(0) {}

    ~FrameAllocator() override = default;

    void* Allocate(usize size, usize alignment = alignof(std::max_align_t)) override {
        usize current = reinterpret_cast<usize>(m_Buffer + m_Offset);
        usize aligned = AlignAddress(current, alignment);
        usize advance = aligned - reinterpret_cast<usize>(m_Buffer) + size;

        SHAPE_ASSERT_MSG(advance <= m_Capacity,
            "FrameAllocator overflow: requested {} bytes, capacity {}", advance, m_Capacity);

        m_Offset = advance;
        m_Peak   = m_Offset > m_Peak ? m_Offset : m_Peak;
        return reinterpret_cast<void*>(aligned);
    }

    /// Individual frees are NOPs — this is an arena.
    void Deallocate(void*) override {}

    /// Rewind pointer to zero. Call once per frame/tick.
    void Reset() override { m_Offset = 0; }

    usize Used()     const { return m_Offset; }
    usize Capacity() const { return m_Capacity; }
    usize PeakUsed() const { return m_Peak; }     // high-water mark

private:
    std::unique_ptr<u8[]> m_OwnedBuffer;
    u8*   m_Buffer   = nullptr;
    usize m_Capacity = 0;
    usize m_Offset   = 0;
    usize m_Peak     = 0;
};

} // namespace Shape
