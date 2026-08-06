#pragma once

#include "Rendering/Renderer.hpp"
#include "Math/Matrix4.hpp"
#include <SDL3/SDL.h>
#include <vector>
#include <cstdint>

namespace Shape {

struct InstanceData {
    Math::Vector2f position{0.0f, 0.0f};
    f32 rotation = 0.0f;
    Math::Vector2f scale{1.0f, 1.0f};
    RenderColor color{1.0f, 1.0f, 1.0f, 1.0f};
};

class OpenGLRenderer final : public IRenderer {
public:
    OpenGLRenderer() = default;
    ~OpenGLRenderer() override;

    bool Initialize(Window& window) override;
    void Shutdown() override;

    void BeginFrame() override;
    void EndFrame() override;

    void SetClearColor(const RenderColor& color) override;
    void Clear() override;

    // View/Projection matrix setup for 2D Camera
    void SetCameraMatrices(const Math::Matrix4& projection, const Math::Matrix4& view);

    // Immediate procedural geometry drawing primitives
    void DrawLine(const Math::Vector2f& start, const Math::Vector2f& end, const Math::Vector3f& color, f32 thickness = 1.0f) override;
    void DrawPolygon(const Math::Vector2f* vertices, usize vertexCount, const Math::Vector3f& color, bool filled = true) override;
    void DrawCircle(const Math::Vector2f& center, f32 radius, const Math::Vector3f& color, bool filled = true) override;

    // High-performance instanced batch rendering for 10K-100K+ entities
    void DrawInstancedPolygons(const Math::Vector2f* baseVertices, usize vertexCount, const InstanceData* instances, usize instanceCount, bool filled = true);
    void DrawInstancedCircles(f32 radius, const InstanceData* instances, usize instanceCount, bool filled = true);

private:
    bool InitShaders();
    bool InitBuffers();

    SDL_Window* m_sdlWindow = nullptr;
    SDL_GLContext m_glContext = nullptr;
    RenderColor m_clearColor{0.1f, 0.1f, 0.12f, 1.0f};

    Math::Matrix4 m_projectionMatrix = Math::Matrix4::Identity();
    Math::Matrix4 m_viewMatrix = Math::Matrix4::Identity();

    uint32_t m_shaderProgram = 0;
    int32_t m_projLoc = -1;
    int32_t m_viewLoc = -1;

    uint32_t m_vao = 0;
    uint32_t m_vboBase = 0;
    uint32_t m_vboInstance = 0;

    std::vector<Math::Vector2f> m_unitCircleVertices;
};

} // namespace Shape
