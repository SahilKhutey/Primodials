#pragma once

/// @file SpatialGrid.hpp — Uniform-grid spatial hash for O(1) proximity queries.

#include "Core/Platform.hpp"
#include "Core/Assert.hpp"
#include "Math/Include/ShapeEngine/Math/Vector2.hpp"
#include "Simulation/SimulationState.hpp"
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace Shape {

// ─── Cell coordinate ──────────────────────────────────────────────────────────

struct CellCoord {
    i32 x = 0;
    i32 y = 0;

    bool operator==(const CellCoord& o) const noexcept { return x == o.x && y == o.y; }
};

struct CellCoordHash {
    usize operator()(const CellCoord& c) const noexcept {
        const u64 h = (static_cast<u64>(static_cast<u32>(c.x)) << 32)
                    |  static_cast<u64>(static_cast<u32>(c.y));
        return static_cast<usize>(h ^ (h >> 33) ^ (h << 17));
    }
};

// ─── SpatialGrid ─────────────────────────────────────────────────────────────

class SpatialGrid {
public:
    struct Config {
        float      cell_size = 20.0f;
        WorldBounds bounds;
        bool       wrap     = true;
    };

    SpatialGrid();
    explicit SpatialGrid(const Config& cfg);

    void reconfigure(const Config& cfg);
    SHAPE_NODISCARD const Config& config() const noexcept { return m_cfg; }

    // ── Mutation ──────────────────────────────────────────────────────────

    void insert(u32 entity_idx, Math::Vector2f pos);
    void update(u32 entity_idx, Math::Vector2f old_pos, Math::Vector2f new_pos);
    void remove(u32 entity_idx, Math::Vector2f pos);
    void clear() noexcept { m_cells.clear(); m_entity_cell.clear(); }

    // ── Queries ───────────────────────────────────────────────────────────

    void query_circle(Math::Vector2f center, float radius,
                      std::vector<u32>& out) const;

    void query_aabb(Math::Vector2f lo, Math::Vector2f hi,
                    std::vector<u32>& out) const;

    // ── Statistics ────────────────────────────────────────────────────────

    struct Stats {
        usize entity_count   = 0;
        usize occupied_cells = 0;
        float avg_density    = 0.0f;
        usize max_density    = 0;
    };
    SHAPE_NODISCARD Stats stats() const noexcept;

    SHAPE_NODISCARD usize entity_count()   const noexcept { return m_entity_cell.size(); }
    SHAPE_NODISCARD usize occupied_cells() const noexcept { return m_cells.size(); }

private:
    SHAPE_NODISCARD CellCoord world_to_cell(Math::Vector2f pos) const noexcept;
    SHAPE_NODISCARD CellCoord wrap_cell(CellCoord c) const noexcept;

    Config m_cfg;
    i32    m_grid_w = 0;
    i32    m_grid_h = 0;

    using CellMap = std::unordered_map<CellCoord, std::vector<u32>, CellCoordHash>;
    CellMap m_cells;
    std::unordered_map<u32, CellCoord> m_entity_cell;
};

} // namespace Shape
