#include "Physics/SpatialHashGrid.hpp"
#include <cmath>
#include <algorithm>

namespace Shape {

SpatialHashGrid::SpatialHashGrid(f32 cellSize)
    : m_CellSize(cellSize) {
}

void SpatialHashGrid::Insert(Entity entity, const Math::Vector2f& position, f32 radiusOrHalfExtent) {
    i32 minX = static_cast<i32>(std::floor((position.x - radiusOrHalfExtent) / m_CellSize));
    i32 maxX = static_cast<i32>(std::floor((position.x + radiusOrHalfExtent) / m_CellSize));
    i32 minY = static_cast<i32>(std::floor((position.y - radiusOrHalfExtent) / m_CellSize));
    i32 maxY = static_cast<i32>(std::floor((position.y + radiusOrHalfExtent) / m_CellSize));

    for (i32 y = minY; y <= maxY; ++y) {
        for (i32 x = minX; x <= maxX; ++x) {
            u64 key = GetCellKey(x, y);
            m_Grid[key].push_back(entity);
        }
    }
}

void SpatialHashGrid::Query(const Math::Vector2f& position, f32 radiusOrHalfExtent, std::vector<Entity>& outEntities) const {
    i32 minX = static_cast<i32>(std::floor((position.x - radiusOrHalfExtent) / m_CellSize));
    i32 maxX = static_cast<i32>(std::floor((position.x + radiusOrHalfExtent) / m_CellSize));
    i32 minY = static_cast<i32>(std::floor((position.y - radiusOrHalfExtent) / m_CellSize));
    i32 maxY = static_cast<i32>(std::floor((position.y + radiusOrHalfExtent) / m_CellSize));

    for (i32 y = minY; y <= maxY; ++y) {
        for (i32 x = minX; x <= maxX; ++x) {
            u64 key = GetCellKey(x, y);
            auto it = m_Grid.find(key);
            if (it != m_Grid.end()) {
                for (Entity entity : it->second) {
                    if (std::find(outEntities.begin(), outEntities.end(), entity) == outEntities.end()) {
                        outEntities.push_back(entity);
                    }
                }
            }
        }
    }
}

void SpatialHashGrid::Clear() {
    m_Grid.clear();
}

} // namespace Shape
