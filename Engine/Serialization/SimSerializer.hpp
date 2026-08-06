#pragma once

#include "Core/Platform.hpp"
#include "ECS/World.hpp"
#include <string>
#include <vector>

namespace Shape::Serialization {

struct SimHeader {
    char magic[8] = {'S', 'H', 'A', 'P', 'E', 'S', 'I', 'M'};
    u32 version = 1;
    u64 entityCount = 0;
    f64 simulationTime = 0.0;
};

class SimSerializer {
public:
    static bool SaveToFile(World& world, const std::string& filepath, f64 simTime = 0.0);
    static bool LoadFromFile(World& world, const std::string& filepath, f64* outSimTime = nullptr);

    static bool SaveToMemory(World& world, std::vector<u8>& outBuffer, f64 simTime = 0.0);
    static bool LoadFromMemory(World& world, const std::vector<u8>& inBuffer, f64* outSimTime = nullptr);
};

} // namespace Shape::Serialization
