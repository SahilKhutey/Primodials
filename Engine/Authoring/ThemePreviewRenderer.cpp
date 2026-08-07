#include "ThemePreviewRenderer.h"
#include "Core/Logger.hpp"

#include <fstream>
#include <filesystem>

namespace ShapeEngine {

    namespace fs = std::filesystem;

    void ThemePreviewRenderer::clearCache() {}

    uint32_t ThemePreviewRenderer::getOrCreateTexture(const std::string& themeId, const Themes::Theme& theme, int size) {
        (void)themeId;
        (void)theme;
        (void)size;
        return 0;
    }

    void ThemePreviewRenderer::render(const Themes::Theme& theme, const Shape::Math::Vec2f& size) {
        (void)theme;
        (void)size;
    }

    bool ThemePreviewRenderer::savePreviewJpg(const Themes::Theme& theme, const std::string& path, int width, int height) {
        (void)theme;
        (void)width;
        (void)height;

        fs::path p(path);
        if (p.has_parent_path() && !p.parent_path().empty()) {
            fs::create_directories(p.parent_path());
        }

        std::ofstream f(path, std::ios::binary);
        if (!f.is_open()) return false;
        f << "dummy_image_data";
        return true;
    }

} // namespace ShapeEngine
