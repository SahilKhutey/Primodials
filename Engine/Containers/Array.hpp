#pragma once

/// @file Array.hpp
/// @brief Fixed-size stack-allocated array with bounds checking.
///
/// Drop-in replacement for std::array with SHAPE_ASSERT bounds checking.
/// T must be trivially constructible/destructible for zero-overhead iteration.
///
/// Usage:
///   Shape::Array<int, 8> buf;
///   buf[0] = 42;
///   for (auto x : buf) { ... }

#include "Core/Platform.hpp"
#include "Core/Assert.hpp"
#include <iterator>
#include <algorithm>
#include <initializer_list>

namespace Shape {

template<typename T, usize N>
struct Array {
    static_assert(N > 0, "Array size must be > 0");

    T m_data[N]{};

    // ── Element access ────────────────────────────────────────────────────────

    SHAPE_NODISCARD SHAPE_INLINE T& operator[](usize i) noexcept {
        SHAPE_ASSERT(i < N);
        return m_data[i];
    }
    SHAPE_NODISCARD SHAPE_INLINE const T& operator[](usize i) const noexcept {
        SHAPE_ASSERT(i < N);
        return m_data[i];
    }

    SHAPE_NODISCARD SHAPE_INLINE T& Front()             noexcept { return m_data[0]; }
    SHAPE_NODISCARD SHAPE_INLINE const T& Front()  const noexcept { return m_data[0]; }
    SHAPE_NODISCARD SHAPE_INLINE T& Back()              noexcept { return m_data[N - 1]; }
    SHAPE_NODISCARD SHAPE_INLINE const T& Back()   const noexcept { return m_data[N - 1]; }
    SHAPE_NODISCARD SHAPE_INLINE T* Data()               noexcept { return m_data; }
    SHAPE_NODISCARD SHAPE_INLINE const T* Data()   const noexcept { return m_data; }

    // ── Size / capacity ───────────────────────────────────────────────────────

    SHAPE_NODISCARD static constexpr usize Size()     noexcept { return N; }
    SHAPE_NODISCARD static constexpr usize Capacity() noexcept { return N; }
    SHAPE_NODISCARD static constexpr bool  Empty()    noexcept { return N == 0; }

    // ── Fill / swap ───────────────────────────────────────────────────────────

    SHAPE_INLINE void Fill(const T& value) noexcept {
        for (usize i = 0; i < N; ++i) m_data[i] = value;
    }

    SHAPE_INLINE void Swap(Array& other) noexcept {
        for (usize i = 0; i < N; ++i) {
            T tmp       = m_data[i];
            m_data[i]  = other.m_data[i];
            other.m_data[i] = tmp;
        }
    }

    // ── Iteration ─────────────────────────────────────────────────────────────

    SHAPE_NODISCARD T*       begin()       noexcept { return m_data; }
    SHAPE_NODISCARD const T* begin() const noexcept { return m_data; }
    SHAPE_NODISCARD T*       end()         noexcept { return m_data + N; }
    SHAPE_NODISCARD const T* end()   const noexcept { return m_data + N; }

    // ── Comparison ────────────────────────────────────────────────────────────

    SHAPE_NODISCARD bool operator==(const Array& o) const noexcept {
        for (usize i = 0; i < N; ++i) if (m_data[i] != o.m_data[i]) return false;
        return true;
    }
    SHAPE_NODISCARD bool operator!=(const Array& o) const noexcept {
        return !(*this == o);
    }
};

} // namespace Shape
