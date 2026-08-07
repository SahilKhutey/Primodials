#pragma once

#include "ECS/World.hpp"
#include "Physics/SpatialHashGrid.hpp"

namespace Shape {

class InfluenceSystem {
public:
    InfluenceSystem() = default;
    ~InfluenceSystem() = default;

    void Update(World& world, const SpatialHashGrid& spatialGrid);
};

} // namespace Shape
