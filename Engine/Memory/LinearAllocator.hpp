#pragma once

/// @file LinearAllocator.hpp
/// @brief Linear (bump-pointer) allocator over a pre-allocated buffer.
///
/// Allocation: O(1) bump pointer increment.
/// Deallocation: individual frees are a no-op; only Reset() clears all.
/// Use when: you need fast sequential allocations and batch-free all at once.
///
/// NOT thread-safe. Use one per thread or per system.

#include "Memory/Allocator.hpp"
#include "Core/Assert.hpp"
#include <cstring>

namespace Shape {

class LinearAllocator final : public Allocator {
public:
    /// Creates a linear allocator over an externally-owned buffer.
    LinearAllocator(void* buffer, usize capacity) noexcept
        : m_buffer(static_cast<u8*>(buffer))
        , m_capacity(capacity)
        , m_offset(0) {
        SHAPE_ASSERT(buffer != nullptr);
        SHAPE_ASSERT(capacity > 0);
    }

    ~LinearAllocator() noexcept override = default;

    // Non-copyable, non-movable (owns position in external buffer)
    LinearAllocator(const LinearAllocator&)            = delete;
    LinearAllocator& operator=(const LinearAllocator&) = delete;
    LinearAllocator(LinearAllocator&&)                 = delete;
    LinearAllocator& operator=(LinearAllocator&&)      = delete;

    // ── Allocate ──────────────────────────────────────────────────────────────

    SHAPE_NODISCARD void* Allocate(usize size, usize alignment = 8) noexcept override {
        if (size == 0) return nullptr;

        // Align up the current offset
        const usize aligned = AlignUp(m_offset, alignment);
        if (aligned + size > m_capacity) return nullptr; // Out of memory

        void* ptr = m_buffer + aligned;
        m_offset = aligned + size;
        m_peakOffset = m_offset > m_peakOffset ? m_offset : m_peakOffset;
        return ptr;
    }

    /// Free is a no-op for linear allocators.
    void Free(void* /*ptr*/, usize /*size*/ = 0) noexcept override {}

    /// Resize is unsupported — allocate a new block instead.
    SHAPE_NODISCARD void* Reallocate(void* /*ptr*/, usize /*oldSize*/,
                                      usize /*newSize*/, usize /*alignment*/ = 8) noexcept override {
        return nullptr; // Not supported
    }

    // ── Control ───────────────────────────────────────────────────────────────

    /// Reset the allocator to the beginning (frees all allocations).
    void Reset() noexcept {
        m_offset = 0;
    }

    /// Save/restore current offset (scratch mark pattern).
    SHAPE_NODISCARD usize Mark()   const noexcept { return m_offset; }
    void Restore(usize mark) noexcept {
        SHAPE_ASSERT(mark <= m_offset);
        m_offset = mark;
    }

    // ── Stats ─────────────────────────────────────────────────────────────────

    SHAPE_NODISCARD usize Used()      const noexcept { return m_offset; }
    SHAPE_NODISCARD usize Remaining() const noexcept { return m_capacity - m_offset; }
    SHAPE_NODISCARD usize Capacity()  const noexcept { return m_capacity; }
    SHAPE_NODISCARD usize PeakUsed()  const noexcept { return m_peakOffset; }

private:
    static usize AlignUp(usize n, usize align) noexcept {
        return (n + align - 1) & ~(align - 1);
    }

    u8*   m_buffer     = nullptr;
    usize m_capacity   = 0;
    usize m_offset     = 0;
    usize m_peakOffset = 0;
};

} // namespace Shape
