#pragma once

/// @file TypeInfo.hpp
/// @brief Runtime type metadata for ECS2 component registration.
///
/// TypeInfo is created once per component type during ComponentRegistry::register_type<T>()
/// and never changes. All pointers are stable for the lifetime of the program.

#include "Core/Platform.hpp"
#include <cstring>

namespace Shape::ECS2 {

/// Vtable-style type operations for a component type.
struct TypeOps {
    void (*default_construct)(void* dst);              ///< Placement default-construct
    void (*destruct)(void* ptr);                        ///< Destroy in-place
    void (*move_construct)(void* dst, void* src);       ///< Placement move-construct + destroy src
    void (*copy_construct)(void* dst, const void* src); ///< Placement copy-construct
};

/// Full metadata for a registered component type.
struct TypeInfo {
    u32         id;          ///< Unique ComponentId assigned at registration
    usize       size;        ///< sizeof(T)
    usize       alignment;   ///< alignof(T)
    const char* name;        ///< Human-readable name (from __func__ or SHAPE_REFLECT)
    TypeOps     ops;         ///< Type operations vtable
};

/// Build a TypeOps vtable for type T at compile time.
template<typename T>
constexpr TypeOps make_type_ops() noexcept {
    return TypeOps {
        // default_construct
        [](void* dst) { new (dst) T{}; },
        // destruct
        [](void* ptr) { static_cast<T*>(ptr)->~T(); },
        // move_construct
        [](void* dst, void* src) {
            new (dst) T{std::move(*static_cast<T*>(src))};
            static_cast<T*>(src)->~T();
        },
        // copy_construct
        [](void* dst, const void* src) {
            new (dst) T{*static_cast<const T*>(src)};
        }
    };
}

} // namespace Shape::ECS2
