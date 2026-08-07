#pragma once

#include "Themes/Theme.h"
#include "Math/Vec2.hpp"
#include <string>

namespace ShapeEngine {

    class ThemePreviewRenderer {
    public:
        static void render(const Themes::Theme& theme, const Shape::Math::Vec2f& size);
        
        static bool savePreviewJpg(const Themes::Theme& theme, 
                                    const std::string& path,
                                    int width, int height);
        
        static uint32_t getOrCreateTexture(const std::string& themeId, 
                                             const Themes::Theme& theme,
                                             int size = 256);
        
        static void clearCache();
    };

} // namespace ShapeEngine
