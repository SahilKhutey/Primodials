/**
 * Primordials — O(1) Spatial Hash Grid (C++23)
 *
 * Fast neighbor search and collision filtering.
 *
 * License: MIT
 */

#pragma once

#include <cmath>
#include <cstdint>
#include <unordered_map>
#include <vector>

namespace primordials {

struct CellCoord {
    std::int32_t x;
    std::int32_t y;
    [[nodiscard]] constexpr auto operator<=>(const CellCoord&) const noexcept = default;
};

class SpatialHash {
public:
    explicit SpatialHash(float cell_size, std::size_t initial_capacity = 1024)
        : cell_size_(cell_size)
        , inv_cell_size_(1.0f / cell_size) {
        cells_.reserve(initial_capacity);
    }

    void clear() noexcept {
        for (auto& [key, entries] : cells_) {
            entries.clear();
        }
    }

    void insert(std::uint32_t entity_id, float x, float y) noexcept {
        const CellCoord cell = cell_of(x, y);
        const Key key = pack(cell);
        cells_[key].push_back({entity_id, x, y});
    }

    template<typename Callback>
    void query_radius(float cx, float cy, float radius, Callback&& callback) const noexcept {
        const float r_sq = radius * radius;
        const int range = static_cast<int>(std::ceil(radius * inv_cell_size_));
        const CellCoord center = cell_of(cx, cy);

        for (int dx = -range; dx <= range; ++dx) {
            for (int dy = -range; dy <= range; ++dy) {
                const CellCoord cell{center.x + dx, center.y + dy};
                const Key key = pack(cell);

                auto it = cells_.find(key);
                if (it == cells_.end()) continue;

                for (const auto& entry : it->second) {
                    const float ddx = entry.x - cx;
                    const float ddy = entry.y - cy;
                    if (ddx * ddx + ddy * ddy <= r_sq) {
                        if (!callback(entry.entity_id)) return;
                    }
                }
            }
        }
    }

private:
    struct Entry {
        std::uint32_t entity_id;
        float x, y;
    };

    using Key = std::uint64_t;

    [[nodiscard]] constexpr CellCoord cell_of(float x, float y) const noexcept {
        return CellCoord{
            static_cast<std::int32_t>(std::floor(x * inv_cell_size_)),
            static_cast<std::int32_t>(std::floor(y * inv_cell_size_))
        };
    }

    [[nodiscard]] constexpr Key pack(CellCoord c) const noexcept {
        return (static_cast<Key>(static_cast<std::uint32_t>(c.x)) << 32)
             |  static_cast<Key>(static_cast<std::uint32_t>(c.y));
    }

    float cell_size_;
    float inv_cell_size_;
    std::unordered_map<Key, std::vector<Entry>> cells_;
};

} // namespace primordials
