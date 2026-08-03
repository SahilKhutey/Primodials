#include "Simulation/Spatial/SpatialGrid.hpp"
#include <algorithm>
#include <cmath>

namespace Shape {

SpatialGrid::SpatialGrid() : SpatialGrid(Config{}) {}

SpatialGrid::SpatialGrid(const Config& cfg) {
    reconfigure(cfg);
}

void SpatialGrid::reconfigure(const Config& cfg) {
    m_cfg    = cfg;
    m_grid_w = static_cast<i32>(std::ceil(cfg.bounds.width()  / cfg.cell_size));
    m_grid_h = static_cast<i32>(std::ceil(cfg.bounds.height() / cfg.cell_size));
    if (m_grid_w < 1) m_grid_w = 1;
    if (m_grid_h < 1) m_grid_h = 1;
    clear();
}

// ─── Coordinate helpers ───────────────────────────────────────────────────────

CellCoord SpatialGrid::world_to_cell(Math::Vector2f pos) const noexcept {
    if (m_cfg.wrap) {
        const float w = m_cfg.bounds.width();
        const float h = m_cfg.bounds.height();
        while (pos.x <  m_cfg.bounds.min.x) pos.x += w;
        while (pos.x >= m_cfg.bounds.max.x) pos.x -= w;
        while (pos.y <  m_cfg.bounds.min.y) pos.y += h;
        while (pos.y >= m_cfg.bounds.max.y) pos.y -= h;
    }
    const i32 cx = static_cast<i32>(std::floor((pos.x - m_cfg.bounds.min.x) / m_cfg.cell_size));
    const i32 cy = static_cast<i32>(std::floor((pos.y - m_cfg.bounds.min.y) / m_cfg.cell_size));
    return {cx, cy};
}

CellCoord SpatialGrid::wrap_cell(CellCoord c) const noexcept {
    if (!m_cfg.wrap) return c;
    if (c.x < 0)         c.x += m_grid_w;
    if (c.x >= m_grid_w) c.x -= m_grid_w;
    if (c.y < 0)         c.y += m_grid_h;
    if (c.y >= m_grid_h) c.y -= m_grid_h;
    return c;
}

// ─── Mutation ─────────────────────────────────────────────────────────────────

void SpatialGrid::insert(u32 entity_idx, Math::Vector2f pos) {
    const CellCoord c = world_to_cell(pos);
    m_cells[c].push_back(entity_idx);
    m_entity_cell[entity_idx] = c;
}

void SpatialGrid::update(u32 entity_idx, Math::Vector2f old_pos, Math::Vector2f new_pos) {
    const CellCoord old_c = world_to_cell(old_pos);
    const CellCoord new_c = world_to_cell(new_pos);
    if (old_c == new_c) return;

    auto it = m_cells.find(old_c);
    if (it != m_cells.end()) {
        auto& v = it->second;
        v.erase(std::remove(v.begin(), v.end(), entity_idx), v.end());
        if (v.empty()) m_cells.erase(it);
    }

    m_cells[new_c].push_back(entity_idx);
    m_entity_cell[entity_idx] = new_c;
}

void SpatialGrid::remove(u32 entity_idx, Math::Vector2f pos) {
    const CellCoord c = world_to_cell(pos);
    auto it = m_cells.find(c);
    if (it != m_cells.end()) {
        auto& v = it->second;
        v.erase(std::remove(v.begin(), v.end(), entity_idx), v.end());
        if (v.empty()) m_cells.erase(it);
    }
    m_entity_cell.erase(entity_idx);
}

// ─── Queries ──────────────────────────────────────────────────────────────────

void SpatialGrid::query_circle(Math::Vector2f center, float radius,
                                std::vector<u32>& out) const {
    const i32 cell_radius = static_cast<i32>(std::ceil(radius / m_cfg.cell_size)) + 1;
    const CellCoord cc = world_to_cell(center);

    for (i32 dy = -cell_radius; dy <= cell_radius; ++dy) {
        for (i32 dx = -cell_radius; dx <= cell_radius; ++dx) {
            CellCoord c = {cc.x + dx, cc.y + dy};
            if (m_cfg.wrap) {
                c = wrap_cell(c);
            } else {
                if (c.x < 0 || c.y < 0 || c.x >= m_grid_w || c.y >= m_grid_h) continue;
            }
            auto it = m_cells.find(c);
            if (it != m_cells.end()) {
                out.insert(out.end(), it->second.begin(), it->second.end());
            }
        }
    }
}

void SpatialGrid::query_aabb(Math::Vector2f lo, Math::Vector2f hi,
                              std::vector<u32>& out) const {
    const CellCoord lo_c = world_to_cell(lo);
    const CellCoord hi_c = world_to_cell(hi);
    for (i32 cy = lo_c.y; cy <= hi_c.y; ++cy) {
        for (i32 cx = lo_c.x; cx <= hi_c.x; ++cx) {
            CellCoord c{cx, cy};
            if (m_cfg.wrap) c = wrap_cell(c);
            auto it = m_cells.find(c);
            if (it != m_cells.end()) {
                out.insert(out.end(), it->second.begin(), it->second.end());
            }
        }
    }
}

// ─── Stats ────────────────────────────────────────────────────────────────────

SpatialGrid::Stats SpatialGrid::stats() const noexcept {
    Stats s;
    s.entity_count   = m_entity_cell.size();
    s.occupied_cells = m_cells.size();
    if (!m_cells.empty()) {
        usize total = 0;
        for (const auto& [_, v] : m_cells) {
            total        += v.size();
            s.max_density = std::max(s.max_density, v.size());
        }
        s.avg_density = static_cast<float>(total) / static_cast<float>(m_cells.size());
    }
    return s;
}

} // namespace Shape
