#pragma once

#include "Network/Protocol/InputCommand.hpp"
#include "ECS/World.hpp"
#include <vector>
#include <unordered_map>

namespace Shape::Network {

enum class LockstepState {
    WaitingForReady,
    Syncing,
    Running,
    Paused
};

struct LockstepConfig {
    u32 targetTickRate = 20;
    u32 maxTickDrift = 5;
    bool enableDesyncCheck = true;
};

class LockstepManager {
public:
    LockstepManager() = default;
    ~LockstepManager() = default;

    void Initialize(World* world, const LockstepConfig& config = {});
    void Shutdown();

    void AddLocalPlayer(u64 playerId);
    void AddRemotePlayer(u64 playerId, const std::string& name);
    void RemovePlayer(u64 playerId);

    void SubmitInput(const PlayerInput& input);
    void ReceiveInput(u64 fromPlayer, const InputBatch& batch);
    void Update(f32 deltaTime);

    LockstepState GetState() const { return m_state; }
    u32 GetCurrentTick() const { return m_currentTick; }
    usize GetPlayerCount() const { return m_players.size(); }
    bool IsReady() const;

private:
    struct Player {
        u64 id = 0;
        std::string name;
        bool isLocal = false;
        bool isReady = false;
        std::unordered_map<u32, PlayerInput> inputsByTick;
    };

    World* m_world = nullptr;
    LockstepConfig m_config;
    LockstepState m_state = LockstepState::WaitingForReady;
    u32 m_currentTick = 0;
    f32 m_tickAccumulator = 0.0f;
    f32 m_tickInterval = 0.05f;

    std::unordered_map<u64, Player> m_players;
    u64 m_localPlayerId = 0;
    std::vector<PlayerInput> m_pendingInputs;
};

} // namespace Shape::Network
