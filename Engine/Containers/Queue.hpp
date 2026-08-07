#pragma once

/// @file Queue.hpp
/// @brief FIFO queue backed by a fixed-capacity ring buffer.

#include "Core/Platform.hpp"
#include "Core/Assert.hpp"

namespace Shape {

template<typename T, usize Capacity>
class Queue {
    static_assert(Capacity > 0 && (Capacity & (Capacity - 1)) == 0,
                  "Queue capacity must be a power of two");

public:
    Queue() noexcept : m_head(0), m_tail(0) {}

    void Enqueue(const T& val) noexcept {
        SHAPE_ASSERT(!Full());
        m_buffer[m_tail++ & MASK] = val;
    }

    void Enqueue(T&& val) noexcept {
        SHAPE_ASSERT(!Full());
        m_buffer[m_tail++ & MASK] = std::move(val);
    }

    T Dequeue() noexcept {
        SHAPE_ASSERT(!Empty());
        return std::move(m_buffer[m_head++ & MASK]);
    }

    SHAPE_NODISCARD const T& Front() const noexcept {
        SHAPE_ASSERT(!Empty());
        return m_buffer[m_head & MASK];
    }

    SHAPE_NODISCARD bool  Empty()    const noexcept { return m_head == m_tail; }
    SHAPE_NODISCARD bool  Full()     const noexcept { return Size() == Capacity; }
    SHAPE_NODISCARD usize Size()     const noexcept { return m_tail - m_head; }
    static constexpr usize MaxCapacity() noexcept   { return Capacity; }

    void Clear() noexcept { m_head = m_tail = 0; }

private:
    static constexpr usize MASK = Capacity - 1;
    T     m_buffer[Capacity]{};
    usize m_head;
    usize m_tail;
};

} // namespace Shape
