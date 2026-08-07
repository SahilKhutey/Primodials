#pragma once

#include "Core/Platform.hpp"
#include "Math/Vec4.hpp"
#include <string>
#include <vector>

namespace Shape::Themes {

struct Theme {
    std::string id;
    std::string displayName;
    std::string description;
    std::string author;

    Math::Vec4f backgroundTopColor{0.02f, 0.03f, 0.08f, 1.0f};
    Math::Vec4f backgroundBottomColor{0.01f, 0.01f, 0.04f, 1.0f};
    Math::Vec4f vignetteColor{0.0f, 0.0f, 0.05f, 0.6f};
    f32 vignetteIntensity = 0.6f;

    std::vector<Math::Vec4f> speciesPalette;
    Math::Vec4f accentColor{0.3f, 0.9f, 1.0f, 1.0f};

    f32 glowIntensity = 0.8f;
    f32 particleDensity = 0.5f;
    f32 motionSpeed = 0.7f;
    f32 populationDensity = 0.6f;

    std::string ambientAudioPath;
    f32 audioVolume = 0.25f;
};

} // namespace Shape::Themes
