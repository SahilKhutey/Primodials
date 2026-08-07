#pragma once

#include "Renderer.hpp"
#include <SDL3/SDL.h>

namespace Shape {

class SDL3Renderer final : public IRenderer {
public:
    SDL3Renderer() = default;
    ~SDL3Renderer() override;

    bool Initialize(Window& window) override;
    void Shutdown() override;

    void BeginFrame() override;
    void EndFrame() override;

    void SetClearColor(const RenderColor& color) override;
    void Clear() override;

    void DrawLine(const Math::Vector2f& start, const Math::Vector2f& end, const Math::Vector3f& color, f32 thickness = 1.0f) override;
    void DrawPolygon(const Math::Vector2f* vertices, usize vertexCount, const Math::Vector3f& color, bool filled = true) override;
    void DrawCircle(const Math::Vector2f& center, f32 radius, const Math::Vector3f& color, bool filled = true) override;

private:
    SDL_Window* m_sdlWindow = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    RenderColor m_clearColor;
};

} // namespace Shape
