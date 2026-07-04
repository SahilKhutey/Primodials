#include "AI/CognitiveSystem.hpp"
#include "ECS/MindComponent.hpp"
#include "ECS/LifecycleComponent.hpp"
#include <algorithm>

namespace Shape {

void CognitiveSystem::Update(World& world, f32 dt) {
    auto entities = world.Query<MindComponent, LifecycleComponent>();

    for (Entity entity : entities) {
        auto& mind = world.GetComponent<MindComponent>(entity);
        auto& lifecycle = world.GetComponent<LifecycleComponent>(entity);

        // 1. Sync needs with metabolism lifecycle states
        mind.hunger = 100.0f - lifecycle.energy;
        
        // Fatigue increments gradually over time
        mind.fatigue += 0.8f * dt;
        mind.fatigue = std::clamp(mind.fatigue, 0.0f, 100.0f);

        // If health is compromised, lower the safety sense
        if (lifecycle.health < 50.0f) {
            mind.safety = std::min(mind.safety, lifecycle.health * 2.0f);
        } else {
            // Safety recovers slowly if not under active threats
            mind.safety += 5.0f * dt;
            mind.safety = std::clamp(mind.safety, 0.0f, 100.0f);
        }

        // 2. Natural decay of active emotional states towards base values
        mind.fear -= 4.0f * dt;
        mind.fear = std::clamp(mind.fear, 0.0f, 100.0f);

        mind.rage -= 2.0f * dt;
        mind.rage = std::clamp(mind.rage, 0.0f, 100.0f);

        mind.wonder -= 3.0f * dt;
        mind.wonder = std::clamp(mind.wonder, 0.0f, 100.0f);

        if (mind.trust > 50.0f) {
            mind.trust -= 1.0f * dt;
        } else if (mind.trust < 50.0f) {
            mind.trust += 1.0f * dt;
        }

        // 3. Process memory decay
        for (auto it = mind.memories.begin(); it != mind.memories.end(); ) {
            it->age += dt;
            if (it->age > 30.0f) {
                it->strength -= 0.1f * dt; // Start decaying strength
            }
            
            if (it->strength <= 0.0f) {
                it = mind.memories.erase(it);
            } else {
                ++it;
            }
        }
    }
}

} // namespace Shape
