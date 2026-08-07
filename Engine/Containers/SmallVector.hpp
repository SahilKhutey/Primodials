#pragma once

/// @file SmallVector.hpp
/// @brief Vector with inline small-buffer optimization.
///
/// Stores up to N elements on the stack; falls back to heap for larger sizes.
/// Optimal for temporary collections that are usually small (e.g., collision
/// candidate lists, neighbor entity queries, GOAP action buffers).
///
/// Usage:
///   SmallVector<EntityId, 8> neighbors; // Stack-allocated up to 8

#include "Core/Platform.hpp"
#include "Core/Assert.hpp"
#include <cstring>
#include <new>
#include <utility>

namespace Shape {

template<typename T, usize InlineCapacity = 8>
class SmallVector {
    static_assert(InlineCapacity > 0, "InlineCapacity must be > 0");

public:
    SmallVector() noexcept : m_data(InlinePtr()), m_size(0), m_capacity(InlineCapacity) {}

    explicit SmallVector(usize n) : SmallVector() { Resize(n); }

    ~SmallVector() noexcept {
        Clear();
        if (!IsInline()) ::operator delete[](m_data);
    }

    SmallVector(const SmallVector& o) : SmallVector() {
        Reserve(o.m_size);
        for (usize i = 0; i < o.m_size; ++i) PushBack(o.m_data[i]);
    }

    SmallVector(SmallVector&& o) noexcept : SmallVector() {
        if (o.IsInline()) {
            for (usize i = 0; i < o.m_size; ++i) {
                new (m_data + i) T{std::move(o.m_data[i])};
                o.m_data[i].~T();
            }
            m_size = o.m_size;
        } else {
            m_data     = o.m_data;
            m_size     = o.m_size;
            m_capacity = o.m_capacity;
            o.m_data     = o.InlinePtr();
            o.m_size     = 0;
            o.m_capacity = InlineCapacity;
        }
    }

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
        if (!IsInline()) ::operator delete[](m_data);
        m_data     = newData;
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

    // ── Access ────────────────────────────────────────────────────────────────

    SHAPE_NODISCARD T& operator[](usize i) noexcept { SHAPE_ASSERT(i < m_size); return m_data[i]; }
    SHAPE_NODISCARD const T& operator[](usize i) const noexcept { SHAPE_ASSERT(i < m_size); return m_data[i]; }

    SHAPE_NODISCARD T*       Data()        noexcept { return m_data; }
    SHAPE_NODISCARD const T* Data()  const noexcept { return m_data; }
    SHAPE_NODISCARD T&       Front()       noexcept { SHAPE_ASSERT(m_size > 0); return m_data[0]; }
    SHAPE_NODISCARD T&       Back()        noexcept { SHAPE_ASSERT(m_size > 0); return m_data[m_size - 1]; }
    SHAPE_NODISCARD usize    Size()  const noexcept { return m_size; }
    SHAPE_NODISCARD usize    Capacity() const noexcept { return m_capacity; }
    SHAPE_NODISCARD bool     Empty() const noexcept { return m_size == 0; }
    SHAPE_NODISCARD bool     IsInline() const noexcept { return m_data == InlinePtr(); }

    T*       begin()       noexcept { return m_data; }
    const T* begin() const noexcept { return m_data; }
    T*       end()         noexcept { return m_data + m_size; }
    const T* end()   const noexcept { return m_data + m_size; }

private:
    T* InlinePtr() noexcept {
        return reinterpret_cast<T*>(m_inline);
    }
    const T* InlinePtr() const noexcept {
        return reinterpret_cast<const T*>(m_inline);
    }

    void Grow() {
        Reserve(m_capacity == 0 ? InlineCapacity : m_capacity * 2);
    }

    alignas(T) u8 m_inline[InlineCapacity * sizeof(T)]{};
    T*    m_data     = nullptr;
    usize m_size     = 0;
    usize m_capacity = 0;
};

} // namespace Shape
