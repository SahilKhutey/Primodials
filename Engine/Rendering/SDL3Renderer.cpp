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

    // Quality: geometry-based line rendering for crisp thick lines
    SDL_SetRenderVSync(m_renderer, 1);
    SDL_SetRenderScale(m_renderer, 1.0f, 1.0f);
    SDL_SetHint(SDL_HINT_RENDER_LINE_METHOD, "3");  // geometry-based lines (SDL3)
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
    if (thickness <= 1.0f) {
        SDL_SetRenderDrawColorFloat(m_renderer, color.x, color.y, color.z, 1.0f);
        SDL_RenderLine(m_renderer, start.x, start.y, end.x, end.y);
        return;
    }

    // Build a thick line as a quad (2 triangles) offset perpendicular to the direction
    const float dx = end.x - start.x;
    const float dy = end.y - start.y;
    const float len = std::sqrt(dx * dx + dy * dy);
    if (len < 0.001f) return;

    const float nx = (-dy / len) * (thickness * 0.5f);  // perpendicular offset
    const float ny = ( dx / len) * (thickness * 0.5f);

    SDL_Vertex verts[4];
    verts[0].position = { start.x - nx, start.y - ny };
    verts[1].position = { start.x + nx, start.y + ny };
    verts[2].position = { end.x   + nx, end.y   + ny };
    verts[3].position = { end.x   - nx, end.y   - ny };
    for (auto& v : verts)
        v.color = { color.x, color.y, color.z, 1.0f };

    const int indices[6] = { 0, 1, 2,  0, 2, 3 };
    SDL_RenderGeometry(m_renderer, nullptr, verts, 4, indices, 6);
}

void SDL3Renderer::DrawPolygon(const Math::Vector2f* vertices, usize vertexCount, const Math::Vector3f& color, bool filled) {
    if (vertexCount < 3) return;

    if (!filled) {
        // Draw crisp thick outline edges using geometry-based thick lines (2px)
        for (usize i = 0; i < vertexCount; ++i) {
            usize next = (i + 1) % vertexCount;
            DrawLine(vertices[i], vertices[next], color, 2.0f);
        }
    } else {
        // Fan triangulation for filled polygon
        for (usize i = 1; i + 1 < vertexCount; ++i) {
            SDL_Vertex v[3];
            v[0].position = { vertices[0].x,   vertices[0].y };
            v[1].position = { vertices[i].x,   vertices[i].y };
            v[2].position = { vertices[i+1].x, vertices[i+1].y };
            v[0].color = v[1].color = v[2].color = { color.x, color.y, color.z, 1.0f };
            SDL_RenderGeometry(m_renderer, nullptr, v, 3, nullptr, 0);
        }
    }
}

void SDL3Renderer::DrawCircle(const Math::Vector2f& center, f32 radius, const Math::Vector3f& color, bool filled) {
    // Adaptive segment count: more segments for larger circles — smooth at all sizes
    // clamp between 24 (tiny nuclei) and 128 (large aura rings)
    const int segments = static_cast<int>(std::clamp(2.0f * 3.14159265f * radius / 2.0f, 24.0f, 128.0f));
    const float step = (2.0f * 3.14159265f) / segments;

    if (!filled) {
        for (int i = 0; i < segments; ++i) {
            const float a1 = i * step;
            const float a2 = (i + 1) * step;
            DrawLine(
                { center.x + radius * cosf(a1), center.y + radius * sinf(a1) },
                { center.x + radius * cosf(a2), center.y + radius * sinf(a2) },
                color, 1.5f);
        }
    } else {
        for (int i = 0; i < segments; ++i) {
            const float a1 = i * step;
            const float a2 = (i + 1) * step;
            SDL_Vertex v[3];
            v[0].position = { center.x,                          center.y                          };
            v[1].position = { center.x + radius * cosf(a1),      center.y + radius * sinf(a1)      };
            v[2].position = { center.x + radius * cosf(a2),      center.y + radius * sinf(a2)      };
            v[0].color = v[1].color = v[2].color = { color.x, color.y, color.z, 1.0f };
            SDL_RenderGeometry(m_renderer, nullptr, v, 3, nullptr, 0);
        }
    }
}

} // namespace Shape
