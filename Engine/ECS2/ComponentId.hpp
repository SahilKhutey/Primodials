#pragma once

/// @file ComponentId.hpp
/// @brief Runtime ComponentId — a u32 assigned once per component type.
///
/// component_id<T>() returns a stable u32 for type T.
/// IDs are assigned in registration order (first call wins).
/// IDs are NOT stable across runs — do NOT serialize them directly.
/// Use component names for serialization (see Reflection/Reflect.hpp).
///
/// DETERMINISM: component_id<T>() is called during world setup, before
/// any simulation ticks. The order must be identical across all peers
/// for multiplayer/lockstep. Use a fixed registration order.

#include "Core/Platform.hpp"
#include <atomic>

namespace Shape::ECS2 {

using ComponentId = u32;
static constexpr ComponentId INVALID_COMPONENT_ID = ~u32{0};
static constexpr usize MAX_COMPONENT_TYPES = 256;

namespace detail {
    // Global counter for component IDs — starts at 0, increments.
    // NOT thread-safe during tick; only called during init.
    inline u32 next_id() noexcept {
        static u32 counter = 0;
        return counter++;
    }
} // namespace detail

/// Returns the stable ComponentId for type T.
/// First call registers the type; subsequent calls return the same id.
template<typename T>
ComponentId component_id() noexcept {
    static const ComponentId id = detail::next_id();
    return id;
}

} // namespace Shape::ECS2
