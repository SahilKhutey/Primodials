#include "LockstepSynchronizer.hpp"

namespace Shape {

void LockstepSynchronizer::Initialize(u32 localPlayerId, const std::vector<u32>& allPlayerIds) {
    m_localPlayerId = localPlayerId;
    m_allPlayerIds = allPlayerIds;
    m_inputQueues.clear();
}

void LockstepSynchronizer::PushInput(u32 playerId, const PlayerInput& input) {
    m_inputQueues[playerId].push_back(input);
}

bool LockstepSynchronizer::CanAdvanceFrame(u32 currentFrame) const {
    if (m_allPlayerIds.empty()) return true; // No network requirement

    for (u32 pid : m_allPlayerIds) {
        auto it = m_inputQueues.find(pid);
        if (it == m_inputQueues.end()) return false;
        
        bool hasInputForFrame = false;
        for (const auto& input : it->second) {
            if (input.frame == currentFrame) {
                hasInputForFrame = true;
                break;
            }
        }
        if (!hasInputForFrame) {
            return false;
        }
    }
    return true;
}

} // namespace Shape
