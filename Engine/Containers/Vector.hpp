#pragma once

/// @file Vector.hpp
/// @brief Dynamic array (heap-backed). Replacement for std::vector.
///
/// Growth factor: 2x on reallocation.
/// Allocator: optionally takes a custom Allocator (defaults to global heap).
///
/// DETERMINISM: do not sort by pointer value; always use deterministic keys.

#include "Core/Platform.hpp"
#include "Core/Assert.hpp"
#include <cstring>
#include <initializer_list>
#include <new>
#include <utility>
#include <algorithm>

namespace Shape {

template<typename T>
class Vector {
public:
    using value_type = T;
    using pointer    = T*;
    using reference  = T&;
    using iterator   = T*;
    using const_iterator = const T*;

    Vector() noexcept = default;

    explicit Vector(usize n) {
        Reserve(n);
        m_size = n;
        for (usize i = 0; i < n; ++i) new (m_data + i) T{};
    }

    Vector(usize n, const T& val) {
        Reserve(n);
        m_size = n;
        for (usize i = 0; i < n; ++i) new (m_data + i) T{val};
    }

    Vector(std::initializer_list<T> il) {
        Reserve(il.size());
        for (const T& v : il) PushBack(v);
    }

    Vector(const Vector& o) {
        Reserve(o.m_size);
        for (usize i = 0; i < o.m_size; ++i) new (m_data + i) T{o.m_data[i]};
        m_size = o.m_size;
    }

    Vector(Vector&& o) noexcept
        : m_data(o.m_data), m_size(o.m_size), m_capacity(o.m_capacity) {
        o.m_data = nullptr; o.m_size = 0; o.m_capacity = 0;
    }

    ~Vector() noexcept { Clear(); ::operator delete[](m_data); }

    Vector& operator=(const Vector& o) {
        if (this == &o) return *this;
        Clear(); Reserve(o.m_size);
        for (usize i = 0; i < o.m_size; ++i) new (m_data + i) T{o.m_data[i]};
        m_size = o.m_size;
        return *this;
    }

    Vector& operator=(Vector&& o) noexcept {
        if (this == &o) return *this;
        Clear(); ::operator delete[](m_data);
        m_data = o.m_data; m_size = o.m_size; m_capacity = o.m_capacity;
        o.m_data = nullptr; o.m_size = 0; o.m_capacity = 0;
        return *this;
    }

    // ── Element access ────────────────────────────────────────────────────────

    SHAPE_NODISCARD SHAPE_INLINE T& operator[](usize i) noexcept {
        SHAPE_ASSERT(i < m_size); return m_data[i];
    }
    SHAPE_NODISCARD SHAPE_INLINE const T& operator[](usize i) const noexcept {
        SHAPE_ASSERT(i < m_size); return m_data[i];
    }

    SHAPE_NODISCARD T& Front()            noexcept { SHAPE_ASSERT(m_size > 0); return m_data[0]; }
    SHAPE_NODISCARD T& Back()             noexcept { SHAPE_ASSERT(m_size > 0); return m_data[m_size-1]; }
    SHAPE_NODISCARD const T& Front() const noexcept { SHAPE_ASSERT(m_size > 0); return m_data[0]; }
    SHAPE_NODISCARD const T& Back()  const noexcept { SHAPE_ASSERT(m_size > 0); return m_data[m_size-1]; }
    SHAPE_NODISCARD T*       Data()        noexcept { return m_data; }
    SHAPE_NODISCARD const T* Data()  const noexcept { return m_data; }

    // ── Size ──────────────────────────────────────────────────────────────────

    SHAPE_NODISCARD usize Size()     const noexcept { return m_size; }
    SHAPE_NODISCARD usize Capacity() const noexcept { return m_capacity; }
    SHAPE_NODISCARD bool  Empty()    const noexcept { return m_size == 0; }

    // ── Mutation ──────────────────────────────────────────────────────────────

    void PushBack(const T& val) {
        if (m_size == m_capacity) Grow();
        new (m_data + m_size) T{val};
        ++m_size;
    }

    void PushBack(T&& val) {
        if (m_size == m_capacity) Grow();
        new (m_data + m_size) T{std::move(val)};
        ++m_size;
    }

    template<typename... Args>
    T& EmplaceBack(Args&&... args) {
        if (m_size == m_capacity) Grow();
        T* p = new (m_data + m_size) T{std::forward<Args>(args)...};
        ++m_size;
        return *p;
    }

    void PopBack() noexcept {
        SHAPE_ASSERT(m_size > 0);
        --m_size;
        m_data[m_size].~T();
    }

    void Clear() noexcept {
        for (usize i = 0; i < m_size; ++i) m_data[i].~T();
        m_size = 0;
    }

    /// Erase element at index (order-preserving). O(n)
    void Erase(usize i) {
        SHAPE_ASSERT(i < m_size);
        m_data[i].~T();
        for (usize j = i; j < m_size - 1; ++j) {
            new (m_data + j) T{std::move(m_data[j + 1])};
            m_data[j + 1].~T();
        }
        --m_size;
    }

    /// Erase by swap-with-last. O(1) — does NOT preserve order.
    void SwapErase(usize i) noexcept {
        SHAPE_ASSERT(i < m_size);
        m_data[i].~T();
        if (i < m_size - 1) {
            new (m_data + i) T{std::move(m_data[m_size - 1])};
            m_data[m_size - 1].~T();
        }
        --m_size;
    }

    void Reserve(usize n) {
        if (n <= m_capacity) return;
        T* newData = static_cast<T*>(::operator new[](n * sizeof(T)));
        for (usize i = 0; i < m_size; ++i) {
            new (newData + i) T{std::move(m_data[i])};
            m_data[i].~T();
        }
        ::operator delete[](m_data);
        m_data = newData;
        m_capacity = n;
    }

    void Resize(usize n) {
        if (n < m_size) {
            for (usize i = n; i < m_size; ++i) m_data[i].~T();
            m_size = n;
        } else {
            Reserve(n);
            for (usize i = m_size; i < n; ++i) new (m_data + i) T{};
            m_size = n;
        }
    }

    // ── Iteration ─────────────────────────────────────────────────────────────

    iterator       begin()       noexcept { return m_data; }
    const_iterator begin() const noexcept { return m_data; }
    iterator       end()         noexcept { return m_data + m_size; }
    const_iterator end()   const noexcept { return m_data + m_size; }

private:
    void Grow() {
        Reserve(m_capacity == 0 ? 4 : m_capacity * 2);
    }

    T*    m_data     = nullptr;
    usize m_size     = 0;
    usize m_capacity = 0;
};

} // namespace Shape
