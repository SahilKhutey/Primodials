#include "ECS/CareerSystem.hpp"
#include "ECS/CareerComponent.hpp"
#include "ECS/AIComponent.hpp"
#include "ECS/GeometryComponent.hpp"
#include <algorithm>

namespace Shape {

void CareerSystem::Update(World& world, f32 dt) {
    auto entities = world.Query<CareerComponent, AIComponent>();

    for (Entity entity : entities) {
        auto& career = world.GetComponent<CareerComponent>(entity);
        auto& ai = world.GetComponent<AIComponent>(entity);

        // 1. Train and grow proficiency based on current behavior task practice
        if (ai.currentAction == AIAction::Gather) {
            career.skillSurvival += 0.1f * dt;
            career.skillSurvival = std::clamp(career.skillSurvival, 1.0f, 10.0f);
        } else if (ai.currentAction == AIAction::Explore) {
            career.skillExploration += 0.1f * dt;
            career.skillExploration = std::clamp(career.skillExploration, 1.0f, 10.0f);
        }

        // 2. Shape Affinity: Dynamic interest adjustments based on vertex geometries
        if (world.HasComponent<GeometryComponent>(entity)) {
            auto& geom = world.GetComponent<GeometryComponent>(entity);
            if (geom.sideCount == 3) {
                // Triangles naturally prefer Military tasks
                career.interestMilitary = 2.5f;
            } else if (geom.sideCount == 4) {
                // Squares prefer construction & engineering tasks
                career.interestEngineering = 2.5f;
            } else if (geom.sideCount == 5) {
                // Pentagons prefer logistics & trading tasks
                career.interestEconomy = 2.5f;
            }
        }

        // 3. Promote Career Rank based on maximum proficiency accumulated
        f32 maxSkill = std::max({
            career.skillSurvival,
            career.skillEngineering,
            career.skillScience,
            career.skillEconomy,
            career.skillMilitary,
            career.skillExploration
        });

        if (maxSkill >= 5.0f) {
            career.rank = 3; // Expert
        } else if (maxSkill >= 2.5f) {
            career.rank = 2; // Specialist
        } else {
            career.rank = 1; // Citizen/Apprentice
        }
    }
}

} // namespace Shape
