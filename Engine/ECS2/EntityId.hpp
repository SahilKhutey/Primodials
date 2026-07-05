#pragma once

/// @file EntityId.hpp
/// @brief Entity handle for ECS2 — generation-checked 64-bit ID.
///
/// Layout:
///   bits [0..31]  = index into EntityPool's slot array
///   bits [32..63] = generation counter (wraps at 2^32)
///
/// An EntityId is valid only when its generation matches the slot's
/// generation in the EntityPool. Stale handles (from destroyed entities)
/// return false from World2::valid(e).
///
/// NOTE: Named EntityId to avoid collision with the existing ECS Entity (u32).

#include "Core/Platform.hpp"

namespace Shape::ECS2 {

struct EntityId {
    u32 index      = ~u32{0};
    u32 generation = 0;

    SHAPE_NODISCARD bool is_valid() const noexcept { return index != ~u32{0}; }

    bool operator==(EntityId o) const noexcept {
        return index == o.index && generation == o.generation;
    }
    bool operator!=(EntityId o) const noexcept { return !(*this == o); }
    bool operator<(EntityId o)  const noexcept {
        return index < o.index || (index == o.index && generation < o.generation);
    }

    static constexpr EntityId invalid() noexcept { return EntityId{~u32{0}, 0}; }
};

} // namespace Shape::ECS2
