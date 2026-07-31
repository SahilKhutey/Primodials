// Shape/Simulation/Systems/GroupBehaviorSystem.h
#pragma once

#include "Simulation/ISimulationSystem.hpp"
#include "ECS/World.hpp"
#include "ECS/FlockComponent.hpp"
#include "ECS/TransformComponent.hpp"
#include "Physics/RigidBodyComponent.hpp"
#include <string_view>
#include <vector>

namespace Shape::Simulation {

class GroupBehaviorSystem : public ISimulationSystem {
public:
    GroupBehaviorSystem() = default;
    explicit GroupBehaviorSystem(World* world);
    virtual ~GroupBehaviorSystem() = default;

    void set_world(World* world) noexcept { m_world = world; }
    void update(const TickContext& ctx) override;
    std::string_view name() const override { return "GroupBehaviorSystem"; }
    int priority() const override { return 112; }

    static Math::Vector2f compute_separation(const TransformComponent& transform,
                                             const std::vector<std::pair<TransformComponent, RigidBodyComponent>>& neighbors,
                                             f32 perception_radius);

    static Math::Vector2f compute_alignment(const RigidBodyComponent& rb,
                                            const std::vector<std::pair<TransformComponent, RigidBodyComponent>>& neighbors);

    static Math::Vector2f compute_cohesion(const TransformComponent& transform,
                                           const std::vector<std::pair<TransformComponent, RigidBodyComponent>>& neighbors);

private:
    World* m_world = nullptr;
};

} // namespace Shape::Simulation
