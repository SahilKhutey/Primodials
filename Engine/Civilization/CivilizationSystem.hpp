#pragma once
#include "ECS/World.hpp"

namespace Shape {

class CivilizationSystem {
public:
    void Update(World& world, f32 dt);
    
private:
    void ProcessLogistics(World& world, f32 dt);
    void ProcessTechnology(World& world, f32 dt);
};

} // namespace Shape
