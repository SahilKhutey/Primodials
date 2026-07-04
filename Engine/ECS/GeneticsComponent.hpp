#pragma once

#include "Core/Platform.hpp"
#include "Simulation/DeterministicRng.hpp"
#include <array>
#include <algorithm>

namespace Shape {

struct GeneticsComponent {
    // 12 genes representing physiological and behavioral traits
    // 0: Target Side Count (Vertices)
    // 1: Radius Scale
    // 2: Bounciness (Restitution)
    // 3: Maximum Speed Force
    // 4-9: Preferences/Affinity for departments: Survival, Engineering, Science, Economy, Military, Exploration
    // 10: Personal Mutation Rate
    // 11: Base Maximum Lifespan
    std::array<f32, 12> genome;

    GeneticsComponent() {
        genome.fill(1.0f);
        // Defaults
        genome[0] = 3.0f;   // Starts as Triangle
        genome[1] = 1.0f;   // Standard Scale
        genome[2] = 0.5f;   // Restitution
        genome[3] = 20.0f;  // Speed Force
        genome[10] = 0.05f; // Mutation Rate
        genome[11] = 80.0f; // Lifespan in seconds
    }

    /// Combine genomes from two parents using uniform crossover.
    /// Requires a DeterministicRng — no implicit global state.
    static GeneticsComponent Crossover(const GeneticsComponent& parentA,
                                       const GeneticsComponent& parentB,
                                       DeterministicRng& rng) {
        GeneticsComponent child;
        for (usize i = 0; i < child.genome.size(); ++i) {
            child.genome[i] = rng.NextBool(0.5f) ? parentA.genome[i] : parentB.genome[i];
        }
        return child;
    }

    /// Nudge genes based on mutation rate gene[10].
    /// Requires a DeterministicRng — no implicit global state.
    void Mutate(DeterministicRng& rng) {
        f32 rate = genome[10];

        for (usize i = 0; i < genome.size(); ++i) {
            if (rng.NextBool(rate)) {
                // Gaussian nudge approximated by sum of two uniforms (Box-Muller alternative)
                f32 nudge = (rng.NextRangeF32(-0.2f, 0.2f) + rng.NextRangeF32(-0.2f, 0.2f)) * 0.5f;
                genome[i] += nudge;

                // Clamp to realistic boundaries
                if (i == 0)                 genome[i] = std::clamp(genome[i],  3.0f, 100.0f);
                else if (i == 1)            genome[i] = std::clamp(genome[i],  0.2f,   5.0f);
                else if (i == 2)            genome[i] = std::clamp(genome[i],  0.0f,   1.0f);
                else if (i == 3)            genome[i] = std::clamp(genome[i],  1.0f, 100.0f);
                else if (i >= 4 && i <= 9)  genome[i] = std::clamp(genome[i],  0.1f,  10.0f);
                else if (i == 10)           genome[i] = std::clamp(genome[i], 0.01f,   0.5f);
                else if (i == 11)           genome[i] = std::clamp(genome[i], 10.0f, 300.0f);
            }
        }
    }
};

} // namespace Shape
