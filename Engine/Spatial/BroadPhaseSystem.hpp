#pragma once

#include "ECS/System.hpp"
#include "ECS/Entity.hpp"
#include "ECS/World.hpp"
#include "Spatial/SpatialHash.hpp"
#include "ECS/TransformComponent.hpp"

namespace Shape::Spatial {

class BroadPhaseSystem final : public System {
public:
    explicit BroadPhaseSystem(f32 cellSize = 50.0f);
    ~BroadPhaseSystem() override = default;

    std::string_view Name() const override { return "BroadPhaseSystem"; }
    ExecutionPhase Phase() const override { return ExecutionPhase::PreSimulation; }

    void Update(World& world, const UpdateContext& ctx) override;

    SpatialHash<Entity>& GetSpatialHash() { return m_spatialHash; }
    const SpatialHash<Entity>& GetSpatialHash() const { return m_spatialHash; }

private:
    SpatialHash<Entity> m_spatialHash;
};

} // namespace Shape::Spatial
