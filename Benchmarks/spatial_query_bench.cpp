#include "Simulation/Spatial/SpatialGrid.hpp"
#include <chrono>
#include <iostream>
#include <vector>

int main() {
    std::cout << "[Spatial Query Benchmark]\n";
    const u32 entityCount = 10000;

    Shape::SpatialGrid::Config cfg;
    cfg.cell_size = 50.0f;
    Shape::SpatialGrid grid(cfg);

    std::vector<Shape::Math::Vector2f> positions;
    positions.reserve(entityCount);
    for (u32 i = 0; i < entityCount; ++i) {
        Shape::Math::Vector2f pos(
            static_cast<f32>((i * 17) % 2000),
            static_cast<f32>((i * 31) % 2000)
        );
        positions.push_back(pos);
        grid.insert(i + 1, pos);
    }

    auto start = std::chrono::high_resolution_clock::now();
    const int queryCount = 1000;
    usize totalNeighbors = 0;
    std::vector<u32> results;

    for (int q = 0; q < queryCount; ++q) {
        Shape::Math::Vector2f queryPos = positions[q % entityCount];
        results.clear();
        grid.query_circle(queryPos, 75.0f, results);
        totalNeighbors += results.size();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    std::cout << "Spatial Queries: " << queryCount << " radius searches on " << entityCount << " entities\n"
              << "Total time: " << duration.count() << " ms (" 
              << (duration.count() / queryCount) << " ms/query)\n"
              << "Total neighbors found: " << totalNeighbors << "\n";

    return 0;
}
