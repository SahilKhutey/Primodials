#pragma once

/// @file SpeciationSystem.hpp
/// @brief Detects divergence in species genomes.
///
/// Execution order: SystemPriority::Evolution (100)
/// Frequency: Every 1200 ticks (~20 seconds)

#include "Core/Platform.hpp"
#include "Simulation/ISimulationSystem.hpp"
#include "ECS2/World2.hpp"

namespace Shape {

class SpeciationSystem final : public ISimulationSystem {
public:
    SpeciationSystem() = default;

    void on_register()   override;
    void on_unregister() override {}
    void update(const TickContext& ctx) override;
    int  priority() const override { return 100; }
    std::string_view name() const override { return "SpeciationSystem"; }

    void set_world(ECS2::World2* w) noexcept { m_world = w; }

    // ── Tuneable parameters ──────────────────────────────────────────────
    float speciation_threshold = 0.5f;   ///< Distance threshold for speciation
    u32   check_interval_ticks = 1200;   ///< Ticks between checks

private:
    ECS2::World2* m_world      = nullptr;
    u64           m_last_check = 0;
};

} // namespace Shape
