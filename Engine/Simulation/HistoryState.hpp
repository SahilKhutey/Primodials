#pragma once

#include "ECS/Entity.hpp"
#include "ShapeEngine/Math/Vector2.hpp"
#include <vector>

namespace Shape {

struct EntitySnapshot {
    Entity entity;
    Math::Vector2f position;
    Math::Vector2f velocity;
    u32 sideCount;
};

struct TickSnapshot {
    u64 tickNumber;
    std::vector<EntitySnapshot> entities;
};

class SimulationHistory {
public:
    SimulationHistory() = default;
    ~SimulationHistory() = default;

    void RecordState(u64 tickNumber, const std::vector<EntitySnapshot>& snapshot) {
        m_History.push_back({tickNumber, snapshot});
    }

    bool GetState(u64 tickNumber, std::vector<EntitySnapshot>& outSnapshot) const {
        for (const auto& snap : m_History) {
            if (snap.tickNumber == tickNumber) {
                outSnapshot = snap.entities;
                return true;
            }
        }
        return false;
    }

    void Clear() {
        m_History.clear();
    }

    usize GetHistorySize() const { return m_History.size(); }

private:
    std::vector<TickSnapshot> m_History;
};

} // namespace Shape
