#pragma once

#include "Core/Platform.hpp"

namespace Shape {

using Entity = u32;

constexpr Entity INVALID_ENTITY = 0xFFFFFFFF;
constexpr u32 MAX_ENTITIES_SHIFT = 20;
constexpr u32 ENTITY_INDEX_MASK = (1 << MAX_ENTITIES_SHIFT) - 1;       // Lower 20 bits for index (up to 1,048,576 entities)
constexpr u32 ENTITY_GENERATION_MASK = (1 << (32 - MAX_ENTITIES_SHIFT)) - 1; // Upper 12 bits for generation

SHAPE_INLINE constexpr u32 GetEntityIndex(Entity entity) {
    return entity & ENTITY_INDEX_MASK;
}

SHAPE_INLINE constexpr u32 GetEntityGeneration(Entity entity) {
    return (entity >> MAX_ENTITIES_SHIFT) & ENTITY_GENERATION_MASK;
}

SHAPE_INLINE constexpr Entity MakeEntity(u32 index, u32 generation) {
    return (index & ENTITY_INDEX_MASK) | ((generation & ENTITY_GENERATION_MASK) << MAX_ENTITIES_SHIFT);
}

} // namespace Shape
