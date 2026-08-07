#pragma once

/// @file Bitset.hpp
/// @brief Fixed-size bitset with O(1) set/clear/test and popcount.
///
/// Backed by u64 words. Supports iteration over set bits.
///
/// Usage:
///   Bitset<1024> active;
///   active.Set(42);
///   if (active.Test(42)) { ... }
///   for (usize bit : active) { ... }  // iterate set bits

#include "Core/Platform.hpp"
#include "Core/Assert.hpp"
#include <bit>

namespace Shape {

template<usize N>
class Bitset {
    static_assert(N > 0, "Bitset size must be > 0");

    static constexpr usize WORDS = (N + 63) / 64;

public:
    Bitset() noexcept { Clear(); }

    // ── Mutation ──────────────────────────────────────────────────────────────

    SHAPE_INLINE void Set(usize i) noexcept {
        SHAPE_ASSERT(i < N);
        m_words[i / 64] |= (u64{1} << (i & 63));
    }

    SHAPE_INLINE void Clear(usize i) noexcept {
        SHAPE_ASSERT(i < N);
        m_words[i / 64] &= ~(u64{1} << (i & 63));
    }

    SHAPE_INLINE void Flip(usize i) noexcept {
        SHAPE_ASSERT(i < N);
        m_words[i / 64] ^= (u64{1} << (i & 63));
    }

    SHAPE_INLINE void Clear() noexcept {
        for (auto& w : m_words) w = 0;
    }

    SHAPE_INLINE void SetAll() noexcept {
        for (auto& w : m_words) w = ~u64{0};
        // Mask off unused bits in the last word
        if constexpr (N & 63) {
            m_words[WORDS - 1] = (u64{1} << (N & 63)) - 1;
        }
    }

    // ── Query ─────────────────────────────────────────────────────────────────

    SHAPE_NODISCARD SHAPE_INLINE bool Test(usize i) const noexcept {
        SHAPE_ASSERT(i < N);
        return (m_words[i / 64] >> (i & 63)) & 1;
    }

    SHAPE_NODISCARD bool None() const noexcept {
        for (const auto& w : m_words) if (w) return false;
        return true;
    }

    SHAPE_NODISCARD bool Any() const noexcept { return !None(); }

    SHAPE_NODISCARD bool All() const noexcept {
        for (usize wi = 0; wi < WORDS - 1; ++wi)
            if (m_words[wi] != ~u64{0}) return false;
        if constexpr (N & 63) {
            return m_words[WORDS-1] == ((u64{1} << (N & 63)) - 1);
        } else {
            return m_words[WORDS-1] == ~u64{0};
        }
    }

    /// Count of set bits (popcount over all words).
    SHAPE_NODISCARD usize Count() const noexcept {
        usize c = 0;
        for (const auto& w : m_words) c += static_cast<usize>(std::popcount(w));
        return c;
    }

    static constexpr usize Size() noexcept { return N; }

    // ── Bitwise ops ───────────────────────────────────────────────────────────

    Bitset operator&(const Bitset& o) const noexcept {
        Bitset r; for (usize i=0;i<WORDS;++i) r.m_words[i]=m_words[i]&o.m_words[i]; return r;
    }
    Bitset operator|(const Bitset& o) const noexcept {
        Bitset r; for (usize i=0;i<WORDS;++i) r.m_words[i]=m_words[i]|o.m_words[i]; return r;
    }
    Bitset operator^(const Bitset& o) const noexcept {
        Bitset r; for (usize i=0;i<WORDS;++i) r.m_words[i]=m_words[i]^o.m_words[i]; return r;
    }
    Bitset operator~() const noexcept {
        Bitset r; for (usize i=0;i<WORDS;++i) r.m_words[i]=~m_words[i]; return r;
    }
    Bitset& operator&=(const Bitset& o) noexcept {
        for (usize i=0;i<WORDS;++i) { m_words[i]&=o.m_words[i]; } return *this;
    }
    Bitset& operator|=(const Bitset& o) noexcept {
        for (usize i=0;i<WORDS;++i) { m_words[i]|=o.m_words[i]; } return *this;
    }

    bool operator==(const Bitset& o) const noexcept {
        for (usize i=0;i<WORDS;++i) if (m_words[i]!=o.m_words[i]) return false;
        return true;
    }

    // ── Set-bit iterator ──────────────────────────────────────────────────────

    class SetBitIterator {
    public:
        SetBitIterator(const Bitset& bs, usize startWord) noexcept
            : m_bs(bs), m_word(startWord), m_current(0) {
            Advance();
        }
        usize operator*() const noexcept { return m_current; }
        SetBitIterator& operator++() noexcept {
            // Clear this bit and advance
            if (m_word < WORDS) {
                u64 w = m_bs.m_words[m_word] & ~((u64{1} << ((m_current & 63) + 1)) - 1);
                while (m_word < WORDS) {
                    if (w) {
                        m_current = m_word * 64 + static_cast<usize>(std::countr_zero(w));
                        return *this;
                    }
                    ++m_word;
                    w = m_word < WORDS ? m_bs.m_words[m_word] : 0;
                }
            }
            m_word = WORDS; // End
            return *this;
        }
        bool operator!=(const SetBitIterator& o) const noexcept {
            return m_word != o.m_word;
        }
    private:
        void Advance() noexcept {
            while (m_word < WORDS) {
                u64 w = m_bs.m_words[m_word];
                if (w) {
                    m_current = m_word * 64 + static_cast<usize>(std::countr_zero(w));
                    return;
                }
                ++m_word;
            }
        }
        const Bitset& m_bs;
        usize m_word;
        usize m_current = 0;
    };

    SetBitIterator begin() const noexcept { return SetBitIterator(*this, 0); }
    SetBitIterator end()   const noexcept { return SetBitIterator(*this, WORDS); }

private:
    u64 m_words[WORDS];
};

} // namespace Shape
