#include "SDL3Renderer.hpp"
#include "Core/Logger.hpp"
#include "Platform/Window.hpp"

namespace Shape {

SDL3Renderer::~SDL3Renderer() {
    Shutdown();
}

bool SDL3Renderer::Initialize(Window& window) {
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        SHAPE_LOG_ERROR("SDL3Renderer: Failed to initialize SDL VIDEO: {}", SDL_GetError());
        return false;
    }

    m_sdlWindow = reinterpret_cast<SDL_Window*>(window.GetNativeWindow());
    if (!m_sdlWindow) {
        SHAPE_LOG_ERROR("SDL3Renderer: Invalid native window handle.");
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_sdlWindow, nullptr);
    if (!m_renderer) {
        SHAPE_LOG_ERROR("SDL3Renderer: Failed to create SDL_Renderer: {}", SDL_GetError());
        return false;
    }

    SDL_SetRenderVSync(m_renderer, 1);
    SHAPE_LOG_INFO("SDL3Renderer: Initialized successfully.");
    return true;
}

void SDL3Renderer::Shutdown() {
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    // Do not destroy m_sdlWindow, it's owned by Shape::Window
    m_sdlWindow = nullptr;
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void SDL3Renderer::BeginFrame() {
    // Any pre-frame setup
}

void SDL3Renderer::EndFrame() {
    SDL_RenderPresent(m_renderer);
}

void SDL3Renderer::SetClearColor(const RenderColor& color) {
    m_clearColor = color;
    SDL_SetRenderDrawColorFloat(m_renderer, color.r, color.g, color.b, color.a);
}

void SDL3Renderer::Clear() {
    SDL_SetRenderDrawColorFloat(m_renderer, m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    SDL_RenderClear(m_renderer);
}

void SDL3Renderer::DrawLine(const Math::Vector2f& start, const Math::Vector2f& end, const Math::Vector3f& color, f32 thickness) {
    (void)thickness;
    SDL_SetRenderDrawColorFloat(m_renderer, color.x, color.y, color.z, 1.0f);
    // SDL3 handles thickness inherently or we simulate it. For now, basic line.
    SDL_RenderLine(m_renderer, start.x, start.y, end.x, end.y);
}

void SDL3Renderer::DrawPolygon(const Math::Vector2f* vertices, usize vertexCount, const Math::Vector3f& color, bool filled) {
    if (vertexCount < 3) return;
    SDL_SetRenderDrawColorFloat(m_renderer, color.x, color.y, color.z, 1.0f);
    
    if (!filled) {
        for (usize i = 0; i < vertexCount; ++i) {
            usize next = (i + 1) % vertexCount;
            SDL_RenderLine(m_renderer, vertices[i].x, vertices[i].y, vertices[next].x, vertices[next].y);
        }
    } else {
        // Simple fan triangulation for filled polygon
        for (usize i = 1; i < vertexCount - 1; ++i) {
            SDL_Vertex v[3];
            v[0].position = {vertices[0].x, vertices[0].y};
            v[1].position = {vertices[i].x, vertices[i].y};
            v[2].position = {vertices[i+1].x, vertices[i+1].y};
            v[0].color = v[1].color = v[2].color = {color.x, color.y, color.z, 1.0f};
            SDL_RenderGeometry(m_renderer, nullptr, v, 3, nullptr, 0);
        }
    }
}

void SDL3Renderer::DrawCircle(const Math::Vector2f& center, f32 radius, const Math::Vector3f& color, bool filled) {
    SDL_SetRenderDrawColorFloat(m_renderer, color.x, color.y, color.z, 1.0f);
    
    // Naive circle drawing
    const int segments = 32;
    const float step = (2.0f * 3.14159265f) / segments;
    
    if (!filled) {
        for (int i = 0; i < segments; ++i) {
            float a1 = i * step;
            float a2 = (i + 1) * step;
            SDL_RenderLine(m_renderer, 
                           center.x + radius * cosf(a1), center.y + radius * sinf(a1),
                           center.x + radius * cosf(a2), center.y + radius * sinf(a2));
        }
    } else {
        for (int i = 0; i < segments; ++i) {
            float a1 = i * step;
            float a2 = (i + 1) * step;
            SDL_Vertex v[3];
            v[0].position = {center.x, center.y};
            v[1].position = {center.x + radius * cosf(a1), center.y + radius * sinf(a1)};
            v[2].position = {center.x + radius * cosf(a2), center.y + radius * sinf(a2)};
            v[0].color = v[1].color = v[2].color = {color.x, color.y, color.z, 1.0f};
            SDL_RenderGeometry(m_renderer, nullptr, v, 3, nullptr, 0);
        }
    }
}

} // namespace Shape
