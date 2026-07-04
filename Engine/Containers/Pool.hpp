#pragma once

/// @file Pool.hpp
/// @brief Fixed-size object pool with O(1) acquire/release.
///
/// Maintains a free-list of pre-allocated slots. No heap allocation after
/// initialization. All slots are the same size (sizeof(T)).
///
/// DETERMINISM: Slot indices are assigned deterministically based on
/// release order, NOT pointer addresses.
///
/// Usage:
///   Pool<Projectile, 512> pool;
///   u32 id = pool.Acquire();
///   pool.Get(id).velocity = {...};
///   pool.Release(id);

#include "Core/Platform.hpp"
#include "Core/Assert.hpp"
#include <new>

namespace Shape {

template<typename T, usize Capacity>
class Pool {
    static_assert(Capacity > 0);

public:
    Pool() noexcept {
        // Initialize free list: 0 → 1 → 2 → ... → Capacity-1 → INVALID
        for (usize i = 0; i < Capacity; ++i) {
            m_freeList[i] = static_cast<u32>(i + 1);
        }
        m_freeList[Capacity - 1] = INVALID;
        m_head = 0;
    }

    ~Pool() noexcept {
        // Destroy all live objects
        for (usize i = 0; i < Capacity; ++i) {
            if (m_live[i]) {
                reinterpret_cast<T*>(m_storage + i * sizeof(T))->~T();
            }
        }
    }

    // ── Acquire ───────────────────────────────────────────────────────────────

    /// Acquire a slot and default-construct the object. Returns the slot index.
    /// Returns INVALID if pool is full.
    u32 Acquire() noexcept {
        if (m_head == INVALID) return INVALID;
        const u32 idx = m_head;
        m_head = m_freeList[idx];
        new (m_storage + idx * sizeof(T)) T{};
        m_live[idx] = true;
        ++m_count;
        return idx;
    }

    template<typename... Args>
    u32 Acquire(Args&&... args) noexcept {
        if (m_head == INVALID) return INVALID;
        const u32 idx = m_head;
        m_head = m_freeList[idx];
        new (m_storage + idx * sizeof(T)) T{std::forward<Args>(args)...};
        m_live[idx] = true;
        ++m_count;
        return idx;
    }

    // ── Release ───────────────────────────────────────────────────────────────

    void Release(u32 idx) noexcept {
        SHAPE_ASSERT(idx < Capacity && m_live[idx]);
        reinterpret_cast<T*>(m_storage + idx * sizeof(T))->~T();
        m_live[idx]  = false;
        m_freeList[idx] = m_head;
        m_head = idx;
        --m_count;
    }

    // ── Access ────────────────────────────────────────────────────────────────

    SHAPE_NODISCARD T& Get(u32 idx) noexcept {
        SHAPE_ASSERT(idx < Capacity && m_live[idx]);
        return *reinterpret_cast<T*>(m_storage + idx * sizeof(T));
    }

    SHAPE_NODISCARD const T& Get(u32 idx) const noexcept {
        SHAPE_ASSERT(idx < Capacity && m_live[idx]);
        return *reinterpret_cast<const T*>(m_storage + idx * sizeof(T));
    }

    SHAPE_NODISCARD bool IsLive(u32 idx) const noexcept {
        return idx < Capacity && m_live[idx];
    }

    // ── Stats ─────────────────────────────────────────────────────────────────

    SHAPE_NODISCARD usize Count()    const noexcept { return m_count; }
    SHAPE_NODISCARD usize Available()const noexcept { return Capacity - m_count; }
    SHAPE_NODISCARD bool  Full()     const noexcept { return m_head == INVALID; }
    static constexpr usize MaxCapacity() noexcept   { return Capacity; }

    static constexpr u32 INVALID = ~u32{0};

private:
    alignas(T) u8 m_storage[Capacity * sizeof(T)]{};
    u32  m_freeList[Capacity]{};
    bool m_live[Capacity]{};
    u32  m_head  = 0;
    usize m_count = 0;
};

} // namespace Shape
