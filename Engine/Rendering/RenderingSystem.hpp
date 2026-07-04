#pragma once
#include "ECS/World.hpp"
#include "Rendering/Renderer.hpp"

namespace Shape {

class RenderingSystem {
public:
    void Render(World& world, IRenderer& renderer, f32 alpha);
};

} // namespace Shape
