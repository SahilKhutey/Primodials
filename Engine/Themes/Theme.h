#pragma once

#include "Math/Vec4.hpp"
#include <string>
#include <vector>

namespace ShapeEngine::Themes {

    struct Theme {
        std::string id;
        std::string displayName;
        std::string description;
        std::string author;
        std::string version;

        Shape::Math::Vec4f backgroundTopColor    = Shape::Math::Vec4f(0.02f, 0.03f, 0.08f, 1.0f);
        Shape::Math::Vec4f backgroundBottomColor = Shape::Math::Vec4f(0.0f, 0.01f, 0.04f, 1.0f);
        Shape::Math::Vec4f vignetteColor         = Shape::Math::Vec4f(0.0f, 0.0f, 0.05f, 0.8f);
        float vignetteIntensity         = 0.4f;
        float backgroundAnimSpeed      = 0.05f;

        std::vector<Shape::Math::Vec4f> speciesPalette;
        Shape::Math::Vec4f accentColor           = Shape::Math::Vec4f(1.0f);
        Shape::Math::Vec4f glowColor             = Shape::Math::Vec4f(0.4f, 0.9f, 1.0f, 1.0f);
        float glowIntensity             = 0.0f;

        float motionSpeed           = 1.0f;
        float populationDensity     = 1.0f;
        float carryingCapacity     = 1.0f;
        float reproductionRate     = 1.0f;
        float mutationRate         = 0.05f;
        float predationRate        = 1.0f;
        bool  enablePredation      = true;
        bool  enableReproduction   = true;

        float particleDensity      = 0.3f;
        std::vector<std::string> particleTypes;

        float cameraDriftSpeed     = 20.0f;
        float cameraDriftAmplitude = 200.0f;
        float cameraZoomMin        = 0.8f;
        float cameraZoomMax        = 1.3f;
        float cameraEventFrequency = 0.05f;
        bool  cameraAutoFollow     = true;

        std::string ambientAudioPath;
        float audioVolume          = 0.25f;
        bool  audioLoop            = true;

        bool enableBloom           = false;
        float bloomThreshold       = 0.7f;
        float bloomIntensity       = 0.4f;
        bool enableFilmGrain       = false;
        float filmGrainIntensity   = 0.05f;
    };

} // namespace ShapeEngine::Themes
