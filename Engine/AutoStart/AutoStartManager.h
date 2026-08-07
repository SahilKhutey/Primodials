#pragma once

#include <string>

namespace ShapeEngine {

    class AutoStartManager {
    public:
        static bool enable(const std::string& executablePath, 
                          const std::string& appName = "PolygonalPrimordials");

        static bool disable(const std::string& appName = "PolygonalPrimordials");

        static bool isEnabled(const std::string& appName = "PolygonalPrimordials");

        static std::string getRegistrationPath();
    };

} // namespace ShapeEngine
