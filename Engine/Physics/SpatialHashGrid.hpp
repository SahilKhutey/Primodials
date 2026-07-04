#pragma once

#include "ECS/Entity.hpp"
#include "ShapeEngine/Math/Vector2.hpp"
#include <unordered_map>
#include <vector>

namespace Shape {

class SpatialHashGrid {
public:
    explicit SpatialHashGrid(f32 cellSize = 10.0f);
    ~SpatialHashGrid() = default;

    // Insert entity based on its bounding box
    void Insert(Entity entity, const Math::Vector2f& position, f32 radiusOrHalfExtent);

    // Queries all unique entities overlapping a search box
    void Query(const Math::Vector2f& position, f32 radiusOrHalfExtent, std::vector<Entity>& outEntities) const;

    void Clear();

    f32 GetCellSize() const { return m_CellSize; }

private:
    // Helper to pack 2D grid coordinates (x, y) into a single 64-bit key
    SHAPE_INLINE u64 GetCellKey(i32 x, i32 y) const {
        return (static_cast<u64>(x) << 32) | (static_cast<u64>(y) & 0xFFFFFFFFULL);
    }

    f32 m_CellSize;
    std::unordered_map<u64, std::vector<Entity>> m_Grid;
};

} // namespace Shape
