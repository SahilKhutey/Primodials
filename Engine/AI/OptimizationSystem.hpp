#pragma once

#include "ECS/World.hpp"
#include "Physics/SpatialHashGrid.hpp"

namespace Shape {

class OptimizationSystem {
public:
    OptimizationSystem() = default;
    ~OptimizationSystem() = default;

    void Update(World& world, const SpatialHashGrid& spatialGrid, f32 dt);

private:
    void EvaluateAI(World& world, const SpatialHashGrid& spatialGrid, Entity entity, f32 dt);
    void ExecuteAI(World& world, Entity entity, f32 dt);
};

} // namespace Shape
