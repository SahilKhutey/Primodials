#pragma once

#include "Core/Platform.hpp"
#include "ShapeEngine/Math/Vector2.hpp"
#include "ShapeEngine/Math/Vector3.hpp"

namespace Shape {

class Window;

struct RenderColor {
    f32 r = 0.0f;
    f32 g = 0.0f;
    f32 b = 0.0f;
    f32 a = 1.0f;
};

class IRenderer {
public:
    virtual ~IRenderer() = default;

    virtual bool Initialize(Window& window) = 0;
    virtual void Shutdown() = 0;

    virtual void BeginFrame() = 0;
    virtual void EndFrame() = 0;

    virtual void SetClearColor(const RenderColor& color) = 0;
    virtual void Clear() = 0;

    // Procedural geometry drawing primitives
    virtual void DrawLine(const Math::Vector2f& start, const Math::Vector2f& end, const Math::Vector3f& color, f32 thickness = 1.0f) = 0;
    virtual void DrawPolygon(const Math::Vector2f* vertices, usize vertexCount, const Math::Vector3f& color, bool filled = true) = 0;
    virtual void DrawCircle(const Math::Vector2f& center, f32 radius, const Math::Vector3f& color, bool filled = true) = 0;
};

} // namespace Shape
