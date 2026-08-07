#pragma once

#include "Core/Platform.hpp"
#include "ShapeEngine/Math/Vector2.hpp"
#include <unordered_map>
#include <vector>
#include <cmath>

namespace Shape::Spatial {

template<typename T>
class SpatialHash {
public:
    struct Cell {
        std::vector<T> items;
    };

    explicit SpatialHash(f32 cellSize = 50.0f)
        : m_cellSize(cellSize > 0.1f ? cellSize : 50.0f) {}

    ~SpatialHash() = default;

    void SetCellSize(f32 size) {
        if (size > 0.1f) m_cellSize = size;
    }

    f32 GetCellSize() const { return m_cellSize; }

    u64 HashCell(i32 cx, i32 cy) const {
        return (static_cast<u64>(cx & 0xFFFF) << 32) | static_cast<u64>(cy & 0xFFFF);
    }

    std::pair<i32, i32> GetCellCoord(const Math::Vector2f& pos) const {
        return {
            static_cast<i32>(std::floor(pos.x / m_cellSize)),
            static_cast<i32>(std::floor(pos.y / m_cellSize))
        };
    }

    void Insert(const T& item, const Math::Vector2f& pos) {
        auto [cx, cy] = GetCellCoord(pos);
        m_grid[HashCell(cx, cy)].items.push_back(item);
    }

    std::vector<T> QueryRadius(const Math::Vector2f& center, f32 radius) const {
        std::vector<T> result;
        if (radius <= 0.0f) return result;

        i32 cellRadius = static_cast<i32>(std::ceil(radius / m_cellSize));
        auto [ccx, ccy] = GetCellCoord(center);

        for (i32 dx = -cellRadius; dx <= cellRadius; ++dx) {
            for (i32 dy = -cellRadius; dy <= cellRadius; ++dy) {
                u64 key = HashCell(ccx + dx, ccy + dy);
                auto it = m_grid.find(key);
                if (it != m_grid.end()) {
                    for (const auto& item : it->second.items) {
                        result.push_back(item);
                    }
                }
            }
        }
        return result;
    }

    void Clear() {
        for (auto& [key, cell] : m_grid) {
            cell.items.clear();
        }
    }

    usize GetCellCount() const { return m_grid.size(); }

private:
    f32 m_cellSize = 50.0f;
    std::unordered_map<u64, Cell> m_grid;
};

} // namespace Shape::Spatial
