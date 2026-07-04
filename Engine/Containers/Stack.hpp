#pragma once

/// @file Stack.hpp
/// @brief LIFO stack backed by a fixed-capacity inline array.

#include "Core/Platform.hpp"
#include "Core/Assert.hpp"

namespace Shape {

template<typename T, usize Capacity>
class Stack {
    static_assert(Capacity > 0);

public:
    Stack() noexcept = default;

    void Push(const T& val) noexcept {
        SHAPE_ASSERT(m_size < Capacity);
        m_data[m_size++] = val;
    }

    void Push(T&& val) noexcept {
        SHAPE_ASSERT(m_size < Capacity);
        m_data[m_size++] = std::move(val);
    }

    void Pop() noexcept {
        SHAPE_ASSERT(m_size > 0);
        m_data[--m_size].~T();
    }

    SHAPE_NODISCARD T& Top() noexcept {
        SHAPE_ASSERT(m_size > 0);
        return m_data[m_size - 1];
    }

    SHAPE_NODISCARD const T& Top() const noexcept {
        SHAPE_ASSERT(m_size > 0);
        return m_data[m_size - 1];
    }

    SHAPE_NODISCARD bool  Empty()    const noexcept { return m_size == 0; }
    SHAPE_NODISCARD bool  Full()     const noexcept { return m_size == Capacity; }
    SHAPE_NODISCARD usize Size()     const noexcept { return m_size; }
    static constexpr usize MaxCapacity() noexcept   { return Capacity; }

    void Clear() noexcept {
        while (m_size > 0) m_data[--m_size].~T();
    }

private:
    T     m_data[Capacity]{};
    usize m_size = 0;
};

} // namespace Shape
