#pragma once
#include "Core/Platform.hpp"
#include "ECS/World.hpp"
#include <string>
#include <unordered_map>

namespace Shape {
    class ReplaySystem {
    public:
        // Captures world state to a memory stream buffer
        static void CaptureKeyframe(const World& world, u64 tick);
        
        // Restores the snapshot at targetTick, returns true if successful
        static bool RestoreKeyframe(World& world, u64 targetTick);
        
        static void ClearKeyframes();
        static usize GetKeyframeCount();
        
    private:
        static std::unordered_map<u64, std::string> s_Keyframes;
    };
}
