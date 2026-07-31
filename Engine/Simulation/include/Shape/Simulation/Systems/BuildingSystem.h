// Shape/Simulation/Systems/BuildingSystem.h
#pragma once

#include "Simulation/ISimulationSystem.hpp"
#include "ECS/World.hpp"
#include "ECS/ComponentTypes.hpp"
#include "Math/Include/ShapeEngine/Math/Vector2.hpp"
#include <string_view>
#include <vector>

namespace Shape::Simulation {

enum class BuildingType {
    Depot,
    Farm,
    ResearchLab,
    Outpost
};

struct BuildingComponent {
    static constexpr u32 TypeId = static_cast<u32>(ComponentTypeId::BuildingComponent);
    BuildingType building_type = BuildingType::Depot;
    float health = 100.0f;
    float max_health = 100.0f;
    float construction_progress = 1.0f; // 0.0 to 1.0 (1.0 = fully constructed)
    bool is_operational = true;
    uint32_t faction_id = 0;
    float passive_yield_accumulator = 0.0f;
};

class BuildingSystem : public ISimulationSystem {
public:
    BuildingSystem() = default;
    explicit BuildingSystem(World* world);
    virtual ~BuildingSystem() = default;

    void set_world(World* world) noexcept { m_world = world; }
    void update(const TickContext& ctx) override;
    std::string_view name() const override { return "BuildingSystem"; }
    int priority() const override { return 115; }

    Entity construct_building(BuildingType type, uint32_t faction_id, Math::Vector2f position);
    void repair_building(Entity entity, float amount);

private:
    void process_construction(float dt);
    void process_passive_yields(float dt);

    World* m_world = nullptr;
};

} // namespace Shape::Simulation
