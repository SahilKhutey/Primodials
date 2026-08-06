#include "Network/Lockstep/LockstepManager.hpp"
#include "ECS/TransformComponent.hpp"

namespace Shape::Network {

void LockstepManager::Initialize(World* world, const LockstepConfig& config) {
    m_world = world;
    m_config = config;
    m_tickInterval = 1.0f / static_cast<f32>(m_config.targetTickRate);
    m_state = LockstepState::WaitingForReady;
    m_currentTick = 0;
    m_tickAccumulator = 0.0f;
}

void LockstepManager::Shutdown() {
    m_players.clear();
    m_pendingInputs.clear();
    m_state = LockstepState::WaitingForReady;
}

void LockstepManager::AddLocalPlayer(u64 playerId) {
    Player p;
    p.id = playerId;
    p.name = "LocalPlayer";
    p.isLocal = true;
    p.isReady = true;
    m_players[playerId] = p;
    m_localPlayerId = playerId;
}

void LockstepManager::AddRemotePlayer(u64 playerId, const std::string& name) {
    Player p;
    p.id = playerId;
    p.name = name;
    p.isLocal = false;
    p.isReady = true;
    m_players[playerId] = p;
}

void LockstepManager::RemovePlayer(u64 playerId) {
    m_players.erase(playerId);
}

void LockstepManager::SubmitInput(const PlayerInput& input) {
    m_pendingInputs.push_back(input);
}

void LockstepManager::ReceiveInput(u64 fromPlayer, const InputBatch& batch) {
    auto it = m_players.find(fromPlayer);
    if (it != m_players.end()) {
        for (const auto& in : batch.inputs) {
            it->second.inputsByTick[in.tickNumber] = in;
        }
    }
}

void LockstepManager::Update(f32 deltaTime) {
    if (m_state == LockstepState::WaitingForReady) {
        if (IsReady()) {
            m_state = LockstepState::Running;
        }
        return;
    }

    if (m_state != LockstepState::Running) return;

    m_tickAccumulator += deltaTime;
    while (m_tickAccumulator >= m_tickInterval) {
        m_tickAccumulator -= m_tickInterval;

        for (const auto& in : m_pendingInputs) {
            if (in.type == PlayerInputType::SpawnEntity && m_world) {
                Entity e = m_world->CreateEntity();
                m_world->AddComponent<TransformComponent>(e, TransformComponent{ .position = in.position });
            }
        }
        m_pendingInputs.clear();

        m_currentTick++;
    }
}

bool LockstepManager::IsReady() const {
    if (m_players.empty()) return false;
    for (const auto& [id, p] : m_players) {
        if (!p.isReady) return false;
    }
    return true;
}

} // namespace Shape::Network
