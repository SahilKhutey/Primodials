#pragma once
#include "ECS/World.hpp"
#include <SDL3/SDL.h>
#include "ShapeEngine/Math/Vector2.hpp"

namespace ShapeEditor {

class Renderer2D {
public:
    Renderer2D();
    
    void Render(Shape::World& world, SDL_Renderer* renderer);
    
    // Virtual camera
    Shape::Math::Vector2f cameraOffset = {0.0f, 0.0f};
    f32 cameraZoom = 1.0f;
};

} // namespace ShapeEditor
