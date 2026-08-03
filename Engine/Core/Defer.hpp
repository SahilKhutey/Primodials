#pragma once

/// @file Defer.hpp
/// @brief Scope-exit deferred execution (Go-style defer).
///
/// Usage:
///   FILE* f = fopen("x.txt", "r");
///   SHAPE_DEFER { fclose(f); };   // Runs at scope exit
///
///   int* buf = new int[n];
///   SHAPE_DEFER { delete[] buf; };

#include <utility>

namespace Shape::detail {

template<typename F>
class DeferHolder {
public:
    explicit DeferHolder(F&& f) noexcept : m_func(std::move(f)) {}
    ~DeferHolder() noexcept(noexcept(m_func())) { m_func(); }

    DeferHolder(const DeferHolder&) = delete;
    DeferHolder& operator=(const DeferHolder&) = delete;
    DeferHolder(DeferHolder&&)      = delete;
    DeferHolder& operator=(DeferHolder&&) = delete;

private:
    F m_func;
};

struct DeferTag {};

template<typename F>
DeferHolder<F> operator+(DeferTag, F&& f) {
    return DeferHolder<F>(std::forward<F>(f));
}

} // namespace Shape::detail

/// Defer execution of a lambda/block until the enclosing scope exits.
/// Uses __COUNTER__ to generate a unique variable name per-usage.
#define SHAPE_DEFER \
    [[maybe_unused]] auto _shape_defer_##__COUNTER__ = \
        ::Shape::detail::DeferTag{} + [&]()
