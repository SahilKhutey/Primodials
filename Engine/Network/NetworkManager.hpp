#pragma once

#include "Network/Transport/ITransport.hpp"
#include "Network/Transport/LoopbackTransport.hpp"
#include "Network/Lockstep/LockstepManager.hpp"
#include "Network/Lockstep/RollbackSystem.hpp"
#include <memory>

namespace Shape::Network {

enum class NetworkRole {
    None,
    Host,
    Client
};

class NetworkManager {
public:
    NetworkManager() = default;
    ~NetworkManager() = default;

    bool Initialize(World* world, u64 localPlayerId = 1) {
        m_world = world;
        m_localPlayerId = localPlayerId;
        m_transport = std::make_unique<LoopbackTransport>();
        if (!m_transport->Initialize(7777, 16)) return false;

        m_lockstep.Initialize(world);
        m_rollback.Initialize(world);
        m_lockstep.AddLocalPlayer(localPlayerId);
        m_role = NetworkRole::Host;
        m_initialized = true;
        return true;
    }

    void SubmitInput(const PlayerInput& input) {
        if (!m_initialized) return;
        m_lockstep.SubmitInput(input);
    }

    void Update(f32 deltaTime) {
        if (!m_initialized) return;
        m_transport->Poll();
        while (auto pkt = m_transport->Receive()) {
            (void)pkt;
        }
        m_lockstep.Update(deltaTime);
        if (m_lockstep.GetCurrentTick() % 5 == 0) {
            m_rollback.CaptureSnapshot(m_lockstep.GetCurrentTick());
        }
    }

    NetworkRole GetRole() const { return m_role; }
    bool IsInitialized() const { return m_initialized; }
    LockstepManager& GetLockstep() { return m_lockstep; }
    RollbackSystem& GetRollback() { return m_rollback; }

private:
    World* m_world = nullptr;
    u64 m_localPlayerId = 1;
    NetworkRole m_role = NetworkRole::None;
    std::unique_ptr<ITransport> m_transport;
    LockstepManager m_lockstep;
    RollbackSystem m_rollback;
    bool m_initialized = false;
};

} // namespace Shape::Network
