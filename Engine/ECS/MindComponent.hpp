#pragma once

#include "ShapeEngine/Math/Vector2.hpp"
#include <vector>

namespace Shape {

struct MemoryItem {
    Math::Vector2f position = Math::Vector2f::Zero();
    f32 strength = 0.0f;          // Density/importance multiplier
    f32 age = 0.0f;               // Memory decay tracker
};

struct MindComponent {
    // 1. Needs (0 to 100, where 100 is extreme urgency)
    f32 hunger = 0.0f;
    f32 fatigue = 0.0f;
    f32 safety = 100.0f;          // 100 = perfectly safe, 0 = in extreme danger

    // 2. Emotions (0 to 100)
    f32 fear = 0.0f;
    f32 rage = 0.0f;
    f32 trust = 50.0f;
    f32 wonder = 0.0f;

    // Accumulated Spatial Influence Fields (Refreshed every tick)
    f32 accumulatedFearField = 0.0f;
    f32 accumulatedLeadershipField = 0.0f;
    f32 accumulatedKnowledgeField = 0.0f;

    // 3. Personality Traits (0.0 to 1.0, set by genetics)
    f32 aggression = 0.5f;
    f32 curiosity = 0.5f;
    f32 sociability = 0.5f;

    // 4. Memory buffer
    std::vector<MemoryItem> memories;
    static constexpr usize MAX_MEMORIES = 5;

    MindComponent() = default;

    void AddMemory(const Math::Vector2f& pos, f32 strength) {
        // If memory already exists near this location, refresh it
        for (auto& mem : memories) {
            if ((mem.position - pos).LengthSquared() < 9.0f) {
                mem.strength = strength;
                mem.age = 0.0f;
                return;
            }
        }

        // Add new memory item
        if (memories.size() >= MAX_MEMORIES) {
            memories.erase(memories.begin()); // Evict oldest
        }
        memories.push_back({pos, strength, 0.0f});
    }
};

} // namespace Shape
