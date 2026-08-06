#pragma once

#include "Core/Platform.hpp"
#include "ECS/World.hpp"
#include "ECS/TransformComponent.hpp"
#include <vector>

namespace Shape::Renderer {

struct DensityCell {
    i32 cx = 0;
    i32 cy = 0;
    u32 count = 0;
    f32 density = 0.0f;
};

class HeatmapRenderer {
public:
    explicit HeatmapRenderer(f32 cellSize = 50.0f)
        : m_cellSize(cellSize) {}

    void CalculateDensity(World& world) {
        m_cells.clear();
        auto entities = world.Query<TransformComponent>();
        for (Entity e : entities) {
            const auto& tf = world.GetComponent<TransformComponent>(e);
            i32 cx = static_cast<i32>(tf.position.x / m_cellSize);
            i32 cy = static_cast<i32>(tf.position.y / m_cellSize);

            bool found = false;
            for (auto& cell : m_cells) {
                if (cell.cx == cx && cell.cy == cy) {
                    cell.count++;
                    found = true;
                    break;
                }
            }
            if (!found) {
                m_cells.push_back({cx, cy, 1, 0.0f});
            }
        }

        u32 maxCount = 1;
        for (const auto& cell : m_cells) {
            if (cell.count > maxCount) maxCount = cell.count;
        }

        for (auto& cell : m_cells) {
            cell.density = static_cast<f32>(cell.count) / static_cast<f32>(maxCount);
        }
    }

    const std::vector<DensityCell>& GetDensityCells() const { return m_cells; }
    f32 GetCellSize() const { return m_cellSize; }

private:
    f32 m_cellSize = 50.0f;
    std::vector<DensityCell> m_cells;
};

} // namespace Shape::Renderer
