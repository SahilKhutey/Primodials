#pragma once

/// @file HashMap.hpp
/// @brief Open-addressing hash map with robin-hood probing.
///
/// DETERMINISM WARNING: Iteration order is NOT guaranteed to be insertion order.
/// For deterministic iteration, collect keys into a sorted array first, or use
/// the determinism-safe variant (HashMap with sorted iteration helper).
///
/// Key must be hashable via Hash::HashValue(key) or a provided Hasher.
/// Supports: Insert, Find, Remove, Contains, Clear, Size, ForEach (non-deterministic order).

#include "Core/Platform.hpp"
#include "Core/Assert.hpp"
#include "Core/Hash.hpp"
#include <optional>
#include <utility>
#include <functional>

namespace Shape {

template<typename K, typename V,
         typename Hasher = std::hash<K>>
class HashMap {
public:
    struct Entry {
        K    key;
        V    value;
        bool occupied = false;
        bool tombstone = false;
        i32  probeDistance = 0;
    };

    HashMap() noexcept = default;

    explicit HashMap(usize capacity) {
        Rehash(NextPow2(capacity));
    }

    // ── Insert ────────────────────────────────────────────────────────────────

    V& Insert(const K& key, const V& value) {
        MaybeGrow();
        return InsertInternal(key, value);
    }

    V& Insert(const K& key, V&& value) {
        MaybeGrow();
        return InsertInternal(key, std::move(value));
    }

    template<typename... Args>
    V& Emplace(const K& key, Args&&... args) {
        MaybeGrow();
        return EmplaceInternal(key, std::forward<Args>(args)...);
    }

    // ── Lookup ────────────────────────────────────────────────────────────────

    SHAPE_NODISCARD V* Find(const K& key) noexcept {
        if (m_buckets.Empty()) return nullptr;
        const usize h = HashKey(key);
        for (usize i = 0; i < m_buckets.Size(); ++i) {
            const usize idx = (h + i) & (m_buckets.Size() - 1);
            auto& e = m_buckets[idx];
            if (!e.occupied && !e.tombstone) return nullptr;
            if (e.occupied && e.key == key) return &e.value;
        }
        return nullptr;
    }

    SHAPE_NODISCARD const V* Find(const K& key) const noexcept {
        return const_cast<HashMap*>(this)->Find(key);
    }

    SHAPE_NODISCARD bool Contains(const K& key) const noexcept {
        return Find(key) != nullptr;
    }

    V& operator[](const K& key) {
        if (V* v = Find(key)) return *v;
        return Emplace(key);
    }

    // ── Remove ────────────────────────────────────────────────────────────────

    bool Remove(const K& key) noexcept {
        if (m_buckets.Empty()) return false;
        const usize h = HashKey(key);
        for (usize i = 0; i < m_buckets.Size(); ++i) {
            const usize idx = (h + i) & (m_buckets.Size() - 1);
            auto& e = m_buckets[idx];
            if (!e.occupied && !e.tombstone) return false;
            if (e.occupied && e.key == key) {
                e.occupied  = false;
                e.tombstone = true;
                --m_size;
                return true;
            }
        }
        return false;
    }

    // ── Iteration (non-deterministic order) ──────────────────────────────────

    template<typename Fn>
    void ForEach(Fn&& fn) {
        for (auto& e : m_buckets) {
            if (e.occupied) fn(e.key, e.value);
        }
    }

    template<typename Fn>
    void ForEach(Fn&& fn) const {
        for (const auto& e : m_buckets) {
            if (e.occupied) fn(e.key, e.value);
        }
    }

    // ── Utility ───────────────────────────────────────────────────────────────

    void Clear() noexcept {
        for (auto& e : m_buckets) { e.occupied = false; e.tombstone = false; }
        m_size = 0;
    }

    SHAPE_NODISCARD usize Size()     const noexcept { return m_size; }
    SHAPE_NODISCARD bool  Empty()    const noexcept { return m_size == 0; }
    SHAPE_NODISCARD usize Capacity() const noexcept { return m_buckets.Size(); }

private:
    usize HashKey(const K& key) const noexcept {
        return static_cast<usize>(m_hasher(key));
    }

    V& InsertInternal(const K& key, const V& val) {
        const usize h = HashKey(key);
        for (usize i = 0; i < m_buckets.Size(); ++i) {
            const usize idx = (h + i) & (m_buckets.Size() - 1);
            auto& e = m_buckets[idx];
            if (!e.occupied) {
                e.key       = key;
                e.value     = val;
                e.occupied  = true;
                e.tombstone = false;
                ++m_size;
                return e.value;
            }
            if (e.key == key) {
                e.value = val;
                return e.value;
            }
        }
        SHAPE_ASSERT_MSG(false, "HashMap full");
        return m_buckets[h & (m_buckets.Size() - 1)].value;
    }

    V& InsertInternal(const K& key, V&& val) {
        const usize h = HashKey(key);
        for (usize i = 0; i < m_buckets.Size(); ++i) {
            const usize idx = (h + i) & (m_buckets.Size() - 1);
            auto& e = m_buckets[idx];
            if (!e.occupied) {
                e.key       = key;
                e.value     = std::move(val);
                e.occupied  = true;
                e.tombstone = false;
                ++m_size;
                return e.value;
            }
            if (e.key == key) {
                e.value = std::move(val);
                return e.value;
            }
        }
        SHAPE_ASSERT_MSG(false, "HashMap full");
        return m_buckets[h & (m_buckets.Size() - 1)].value;
    }

    template<typename... Args>
    V& EmplaceInternal(const K& key, Args&&... args) {
        const usize h = HashKey(key);
        for (usize i = 0; i < m_buckets.Size(); ++i) {
            const usize idx = (h + i) & (m_buckets.Size() - 1);
            auto& e = m_buckets[idx];
            if (!e.occupied) {
                e.key       = key;
                e.value     = V{std::forward<Args>(args)...};
                e.occupied  = true;
                e.tombstone = false;
                ++m_size;
                return e.value;
            }
            if (e.key == key) return e.value;
        }
        SHAPE_ASSERT_MSG(false, "HashMap full");
        return m_buckets[h & (m_buckets.Size() - 1)].value;
    }

    void MaybeGrow() {
        if (m_buckets.Empty()) { Rehash(16); return; }
        if (m_size * 4 >= m_buckets.Size() * 3) { // 75% load
            Rehash(m_buckets.Size() * 2);
        }
    }

    void Rehash(usize newCapacity) {
        // Collect all live entries
        Vector<Entry> old;
        for (auto& e : m_buckets) if (e.occupied) old.PushBack(e);
        m_buckets.Clear();
        m_buckets.Resize(newCapacity);
        for (auto& e : m_buckets) e.occupied = false;
        m_size = 0;
        for (auto& e : old) InsertInternal(e.key, std::move(e.value));
    }

    static usize NextPow2(usize n) noexcept {
        if (n == 0) return 16;
        --n; n |= n>>1; n |= n>>2; n |= n>>4; n |= n>>8; n |= n>>16;
        if constexpr (sizeof(usize) == 8) n |= n>>32;
        return n + 1;
    }

    // Pulled in Vector for internal bucket storage
    // (avoids coupling to std::vector for consistency)
    struct BucketArray {
        Entry* data    = nullptr;
        usize  count   = 0;
        void Clear() noexcept { for (usize i=0;i<count;++i) data[i].occupied=false; }
        void Resize(usize n) {
            ::operator delete[](data);
            data  = static_cast<Entry*>(::operator new[](n * sizeof(Entry)));
            count = n;
            for (usize i=0;i<n;++i) data[i] = Entry{};
        }
        bool Empty() const noexcept { return count == 0; }
        usize Size() const noexcept { return count; }
        Entry& operator[](usize i) noexcept { return data[i]; }
        const Entry& operator[](usize i) const noexcept { return data[i]; }
        Entry* begin() noexcept { return data; }
        Entry* end()   noexcept { return data + count; }
        const Entry* begin() const noexcept { return data; }
        const Entry* end()   const noexcept { return data + count; }
        ~BucketArray() noexcept { ::operator delete[](data); }
    };

    BucketArray m_buckets;
    usize       m_size = 0;
    Hasher      m_hasher{};
};

} // namespace Shape
