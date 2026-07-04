#pragma once

/// @file TrackingAllocator.hpp
/// @brief Wrapping allocator that records allocation/deallocation counts and sizes.
///
/// Wraps any other Allocator and adds instrumentation.
/// Use in DEBUG builds to find memory leaks or excessive allocation.
/// In Release, it should be swapped out for the raw allocator directly.
///
/// NOT thread-safe.

#include "Memory/Allocator.hpp"
#include "Core/Logger.hpp"
#include <cstdio>
#include <format>

namespace Shape {

struct AllocationRecord {
    void*       ptr        = nullptr;
    usize       size       = 0;
    usize       alignment  = 0;
    const char* tag        = nullptr; ///< Optional label (not owned)
};

class TrackingAllocator final : public Allocator {
public:
    explicit TrackingAllocator(Allocator& inner, const char* name = "Tracked") noexcept
        : m_inner(inner), m_name(name) {}

    ~TrackingAllocator() noexcept override {
#if SHAPE_LOGGING_ENABLED
        if (m_liveAllocations > 0) {
            Logger::instance().log(LogLevel::Warn, "Memory",
                std::format("[{}] {} live allocation(s) on destruction "
                            "— {} bytes leaked",
                            m_name, m_liveAllocations, m_liveBytes));
        }
#endif
    }

    SHAPE_NODISCARD void* Allocate(usize size, usize alignment = 8) noexcept override {
        void* ptr = m_inner.Allocate(size, alignment);
        if (ptr) {
            ++m_totalAllocations;
            ++m_liveAllocations;
            m_totalBytes  += size;
            m_liveBytes   += size;
            m_peakBytes    = m_liveBytes > m_peakBytes ? m_liveBytes : m_peakBytes;
        }
        return ptr;
    }

    void Free(void* ptr, usize size = 0) noexcept override {
        if (!ptr) return;
        m_inner.Free(ptr, size);
        if (m_liveAllocations > 0) --m_liveAllocations;
        m_liveBytes -= size;
        ++m_totalFrees;
    }

    SHAPE_NODISCARD void* Reallocate(void* ptr, usize oldSize, usize newSize,
                                      usize alignment = 8) noexcept override {
        void* newPtr = m_inner.Reallocate(ptr, oldSize, newSize, alignment);
        if (newPtr) {
            m_totalBytes += newSize > oldSize ? (newSize - oldSize) : 0;
            m_liveBytes  += newSize - oldSize;
        }
        return newPtr;
    }

    // ── Stats ─────────────────────────────────────────────────────────────────

    SHAPE_NODISCARD usize TotalAllocations() const noexcept { return m_totalAllocations; }
    SHAPE_NODISCARD usize TotalFrees()       const noexcept { return m_totalFrees; }
    SHAPE_NODISCARD usize LiveAllocations()  const noexcept { return m_liveAllocations; }
    SHAPE_NODISCARD usize TotalBytesEver()   const noexcept { return m_totalBytes; }
    SHAPE_NODISCARD usize LiveBytes()        const noexcept { return m_liveBytes; }
    SHAPE_NODISCARD usize PeakBytes()        const noexcept { return m_peakBytes; }
    SHAPE_NODISCARD bool  HasLeaks()         const noexcept { return m_liveAllocations > 0; }

    void ReportToLog() const {
#if SHAPE_LOGGING_ENABLED
        Logger::instance().log(LogLevel::Info, "Memory", std::format(
            "[{}] allocs={} frees={} live={} peak={}B total={}B",
            m_name, m_totalAllocations, m_totalFrees,
            m_liveAllocations, m_peakBytes, m_totalBytes));
#endif
    }

private:
    Allocator&  m_inner;
    const char* m_name;

    usize m_totalAllocations = 0;
    usize m_totalFrees       = 0;
    usize m_liveAllocations  = 0;
    usize m_totalBytes       = 0;
    usize m_liveBytes        = 0;
    usize m_peakBytes        = 0;
};

} // namespace Shape
