#pragma once

/// @file SimulationSnapshot.hpp
/// @brief Fast binary serialization of ECS2::World2 for rewinds and saves.

#include "Core/Platform.hpp"
#include "ECS2/World2.hpp"
#include <string>

namespace Shape::Simulation {

class SimulationSnapshot {
public:
    /// Save the current state of the world to a .shsnap file
    static bool save_to_file(const ECS2::World2& world, const std::string& path);

    /// Load the world state from a .shsnap file
    static bool load_from_file(ECS2::World2& world, const std::string& path);
    
    /// Save world state to an in-memory blob for time-scrubbing rewind
    static std::vector<u8> save_to_memory(const ECS2::World2& world);
    
    /// Load world state from an in-memory blob
    static bool load_from_memory(ECS2::World2& world, const std::vector<u8>& blob);
};

} // namespace Shape::Simulation
