#pragma once
#include "Networking/PlayerInput.hpp"
#include <unordered_map>
#include <vector>

namespace Shape {

class LockstepSynchronizer {
public:
    void Initialize(u32 localPlayerId, const std::vector<u32>& allPlayerIds);
    void PushInput(u32 playerId, const PlayerInput& input);
    bool CanAdvanceFrame(u32 currentFrame) const;

private:
    u32 m_localPlayerId = 0;
    std::vector<u32> m_allPlayerIds;
    std::unordered_map<u32, std::vector<PlayerInput>> m_inputQueues;
};

} // namespace Shape
