#include "ReplaySystem.hpp"
#include "Serialization/Serializer.hpp"
#include <sstream>

namespace Shape {

std::map<u64, std::string> ReplaySystem::s_Keyframes;

void ReplaySystem::CaptureKeyframe(const World& world, u64 tick) {
    std::ostringstream oss(std::ios::binary);
    Serializer::SaveWorld(world, oss);
    s_Keyframes[tick] = oss.str();
}

bool ReplaySystem::RestoreKeyframe(World& world, u64 targetTick) {
    auto it = s_Keyframes.find(targetTick);
    if (it == s_Keyframes.end()) {
        return false;
    }
    
    std::istringstream iss(it->second, std::ios::binary);
    return Serializer::LoadWorld(world, iss);
}

void ReplaySystem::ClearKeyframes() {
    s_Keyframes.clear();
}

usize ReplaySystem::GetKeyframeCount() {
    return s_Keyframes.size();
}

} // namespace Shape
