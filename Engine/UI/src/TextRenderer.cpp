// Shape/UI/TextRenderer.cpp
#include "Shape/UI/TextRenderer.h"

namespace Shape::UI {
    struct TextRenderer::Impl {};
    TextRenderer::TextRenderer() : m_impl(std::make_unique<Impl>()) {}
    TextRenderer::~TextRenderer() = default;

    bool TextRenderer::initialize(Rendering::IRenderer& renderer) {
        m_renderer = &renderer;
        return true;
    }

    void TextRenderer::shutdown() {}
    bool TextRenderer::load_default_font() { return true; }
    bool TextRenderer::load_font_from_memory(const uint8_t*, size_t, float) { return true; }

    void TextRenderer::draw_text(const std::string&, const Math::Vec2f&, const TextStyle&) {}
    void TextRenderer::draw_text_3d(const std::string&, const Math::Vec3f&, const Math::Mat4f&, const TextStyle&) {}

    Math::Vec2f TextRenderer::measure_text(const std::string&, const TextStyle&) { return {0,0}; }
    float TextRenderer::line_height(const TextStyle&) { return 14.0f; }
}
