#pragma once

#include "ECS/World.hpp"
#include "Serialization/SimSerializer.hpp"
#include <vector>

namespace Shape::Network {

struct RollbackSnapshot {
    u32 tick = 0;
    std::vector<u8> memoryData;
    f64 simTime = 0.0;
};

class RollbackSystem {
public:
    RollbackSystem() = default;
    ~RollbackSystem() = default;

    void Initialize(World* world, usize historyDepth = 60) {
        m_world = world;
        m_historyDepth = historyDepth;
        m_snapshots.clear();
    }

    void CaptureSnapshot(u32 tick, f64 simTime = 0.0) {
        if (!m_world) return;
        RollbackSnapshot snap;
        snap.tick = tick;
        snap.simTime = simTime;
        if (Serialization::SimSerializer::SaveToMemory(*m_world, snap.memoryData, simTime)) {
            m_snapshots.push_back(std::move(snap));
            if (m_snapshots.size() > m_historyDepth) {
                m_snapshots.erase(m_snapshots.begin());
            }
        }
    }

    bool RestoreToTick(u32 tick) {
        if (!m_world) return false;
        for (const auto& snap : m_snapshots) {
            if (snap.tick == tick) {
                return Serialization::SimSerializer::LoadFromMemory(*m_world, snap.memoryData);
            }
        }
        return false;
    }

    usize GetSnapshotCount() const { return m_snapshots.size(); }
    void Clear() { m_snapshots.clear(); }

private:
    World* m_world = nullptr;
    usize m_historyDepth = 60;
    std::vector<RollbackSnapshot> m_snapshots;
};

} // namespace Shape::Network
