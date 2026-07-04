#pragma once

#include "ECS/World.hpp"
#include "Physics/SpatialHashGrid.hpp"

namespace Shape {

class SentinelSystem {
public:
    SentinelSystem() = default;
    ~SentinelSystem() = default;

    void Update(World& world, const SpatialHashGrid& spatialGrid, f32 dt);
};

} // namespace Shape
