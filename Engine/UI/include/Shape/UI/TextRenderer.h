// Shape/UI/TextRenderer.h
// Bitmap font text rendering on top of OpenGL
#pragma once

#include "Shape/Math/Vec2.hpp"
#include "Shape/Math/Vec3.hpp"
#include "Shape/Math/Color.hpp"
#include "Shape/Math/Matrix4.hpp"
#include "Shape/Rendering/IRenderer.hpp"

#include <cstdint>
#include <string>
#include <memory>

namespace Shape::UI {

    struct TextStyle {
        float size = 14.0f;
        Math::Color color{1, 1, 1, 1};
        bool bold = false;
        bool italic = false;
        bool shadow = false;
        Math::Color shadow_color{0, 0, 0, 0.6f};
        Math::Vec2f shadow_offset{1, 1};
        bool word_wrap = false;
        float max_width = 0;
    };

    class TextRenderer {
    public:
        TextRenderer();
        ~TextRenderer();

        bool initialize(Rendering::IRenderer& renderer);
        void shutdown();

        // Load font from memory (TrueType/OpenType)
        bool load_default_font();
        bool load_font_from_memory(const uint8_t* data, size_t size, float pixel_size);

        // Drawing
        void draw_text(const std::string& text, const Math::Vec2f& pos, const TextStyle& style = {});
        void draw_text_3d(const std::string& text, const Math::Vec3f& pos, const Math::Mat4f& view_proj, const TextStyle& style = {});

        // Measurement
        Math::Vec2f measure_text(const std::string& text, const TextStyle& style = {});
        float line_height(const TextStyle& style = {});

        void set_view_projection(const Math::Mat4f& vp) { m_vp = vp; }

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
        Math::Mat4f m_vp;
        Rendering::IRenderer* m_renderer = nullptr;
    };

} // namespace Shape::UI
