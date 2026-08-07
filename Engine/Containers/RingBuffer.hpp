#pragma once

/// @file RingBuffer.hpp
/// @brief Fixed-capacity lock-free-friendly ring buffer (SPSC).
///
/// Power-of-two capacity for O(1) modulo via bitmasking.
/// NOT thread-safe as written — add atomics for SPSC if needed.
///
/// Usage:
///   RingBuffer<AudioEvent, 256> events;
///   events.Push({...});
///   AudioEvent e; if (events.Pop(e)) { ... }

#include "Core/Platform.hpp"
#include "Core/Assert.hpp"
#include <optional>

namespace Shape {

template<typename T, usize Capacity>
class RingBuffer {
    static_assert(Capacity > 0 && (Capacity & (Capacity - 1)) == 0,
                  "RingBuffer capacity must be a power of two");

public:
    RingBuffer() noexcept : m_head(0), m_tail(0) {}

    // ── Push ──────────────────────────────────────────────────────────────────

    /// Push an element. Returns false if full.
    bool Push(const T& val) noexcept {
        if (Full()) return false;
        m_buffer[m_tail & MASK] = val;
        ++m_tail;
        return true;
    }

    bool Push(T&& val) noexcept {
        if (Full()) return false;
        m_buffer[m_tail & MASK] = std::move(val);
        ++m_tail;
        return true;
    }

    // ── Pop ───────────────────────────────────────────────────────────────────

    /// Pop the oldest element. Returns false if empty.
    bool Pop(T& out) noexcept {
        if (Empty()) return false;
        out = std::move(m_buffer[m_head & MASK]);
        ++m_head;
        return true;
    }

    std::optional<T> Pop() noexcept {
        if (Empty()) return std::nullopt;
        T val = std::move(m_buffer[m_head & MASK]);
        ++m_head;
        return val;
    }

    // ── Peek ──────────────────────────────────────────────────────────────────

    SHAPE_NODISCARD const T* Peek() const noexcept {
        if (Empty()) return nullptr;
        return &m_buffer[m_head & MASK];
    }

    // ── State ─────────────────────────────────────────────────────────────────

    SHAPE_NODISCARD bool  Empty()    const noexcept { return m_head == m_tail; }
    SHAPE_NODISCARD bool  Full()     const noexcept { return Size() == Capacity; }
    SHAPE_NODISCARD usize Size()     const noexcept { return m_tail - m_head; }
    SHAPE_NODISCARD static constexpr usize MaxCapacity() noexcept { return Capacity; }

    void Clear() noexcept { m_head = m_tail = 0; }

private:
    static constexpr usize MASK = Capacity - 1;
    T     m_buffer[Capacity]{};
    usize m_head;
    usize m_tail;
};

} // namespace Shape
