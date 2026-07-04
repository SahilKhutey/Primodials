#pragma once

/// @file HeapAllocator.hpp
/// @brief Standard heap allocator wrapping aligned malloc/free.
///
/// The HeapAllocator is the default fallback when no custom allocator is
/// provided. In production, prefer Arena or Pool allocators for hot paths.

#include "Memory/Allocator.hpp"
#include <cstdlib>
#include <new>

namespace Shape {

class HeapAllocator final : public Allocator {
public:
    HeapAllocator() noexcept = default;
    ~HeapAllocator() noexcept override = default;

    /// Allocate size bytes aligned to alignment. Returns nullptr on failure.
    SHAPE_NODISCARD void* Allocate(usize size, usize alignment = 8) noexcept override {
        if (size == 0) return nullptr;
#if defined(SHAPE_PLATFORM_WINDOWS)
        void* ptr = _aligned_malloc(size, alignment);
#else
        void* ptr = nullptr;
        if (::posix_memalign(&ptr, alignment, size) != 0) ptr = nullptr;
#endif
        if (ptr) {
            m_totalAllocated += size;
            ++m_allocationCount;
        }
        return ptr;
    }

    /// Free a previously allocated block.
    void Free(void* ptr, usize size = 0) noexcept override {
        if (!ptr) return;
#if defined(SHAPE_PLATFORM_WINDOWS)
        _aligned_free(ptr);
#else
        ::free(ptr);
#endif
        m_totalAllocated -= size;
        if (m_allocationCount > 0) --m_allocationCount;
    }

    /// Resize a previously allocated block (may reallocate).
    SHAPE_NODISCARD void* Reallocate(void* ptr, usize oldSize, usize newSize,
                                      usize alignment = 8) noexcept override {
        if (newSize == 0) { Free(ptr, oldSize); return nullptr; }
        if (!ptr) return Allocate(newSize, alignment);
        // Realloc to new size
        void* newPtr = Allocate(newSize, alignment);
        if (newPtr && ptr) {
            const usize copySize = oldSize < newSize ? oldSize : newSize;
            std::memcpy(newPtr, ptr, copySize);
            Free(ptr, oldSize);
        }
        return newPtr;
    }

    // ── Stats ─────────────────────────────────────────────────────────────────

    SHAPE_NODISCARD usize TotalAllocated()  const noexcept { return m_totalAllocated; }
    SHAPE_NODISCARD usize AllocationCount() const noexcept { return m_allocationCount; }

private:
    usize m_totalAllocated  = 0;
    usize m_allocationCount = 0;
};

} // namespace Shape
