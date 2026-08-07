#pragma once
#include "ECS/World.hpp"
#include <string>

namespace Shape {
    class Serializer {
    public:
        static bool SaveWorld(const World& world, const std::string& filepath);
        static bool LoadWorld(World& world, const std::string& filepath);
        
        static bool SaveWorld(const World& world, std::ostream& os);
        static bool LoadWorld(World& world, std::istream& is);
    };
}
