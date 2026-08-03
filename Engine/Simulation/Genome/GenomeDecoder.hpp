#pragma once

/// @file GenomeDecoder.hpp
/// @brief Decodes a CreatureGenome (16 normalised floats) into derived per-creature
///        traits, and maps those traits to a CreatureStats struct used at birth.
///
/// Gene layout (all values normalised to [0,1] by GeneticsSystem):
///   [0] health      [1] energy     [2] speed      [3] mass
///   [4] size        [5] hue        [6] saturation  [7] lightness
///   [8] aggression  [9] fear       [10] metabolism [11] fertility
///   [12] iq         [13] lifespan  [14] resistance [15] regen

#include "Core/Platform.hpp"
#include "Simulation/Components/CreatureComponents.hpp"
#include "ECS/SpeciesSchema.hpp"
#include <algorithm>
#include <cmath>

namespace Shape::Simulation::Genome {

// ─── Delta weights ────────────────────────────────────────────────────────────

struct TraitWeights {
    float health_scale      = 40.0f;    ///< gene[0] → ±40 hp
    float energy_scale      = 30.0f;    ///< gene[1] → ±30 energy
    float speed_scale       = 25.0f;    ///< gene[2] → ±25 units/s
    float mass_scale        = 0.6f;     ///< gene[3] → ±0.6 mass
    float size_scale        = 4.0f;     ///< gene[4] → ±4 circumradius (VISIBLE)
    float hue_scale         = 0.40f;    ///< gene[5] → ±0.4 hue (VISIBLE)
    float sat_scale         = 0.30f;    ///< gene[6] → ±0.3 saturation
    float light_scale       = 0.25f;    ///< gene[7] → ±0.25 lightness
    float aggression_scale  = 0.30f;    ///< gene[8] → ±0.3
    float fear_scale        = 0.25f;    ///< gene[9] → ±0.25
    float metabolism_scale  = 0.40f;    ///< gene[10] → ±0.4 rate
    float fertility_scale   = 0.30f;    ///< gene[11] → ±0.3
    float iq_scale          = 25.0f;    ///< gene[12] → ±25 IQ points
    float lifespan_scale    = 12000.0f; ///< gene[13] → ±12000 ticks
    float resistance_scale  = 0.30f;    ///< gene[14] → ±0.3
    float regen_scale       = 0.05f;    ///< gene[15] → ±0.05 regen/tick
};

// ─── Derived traits ───────────────────────────────────────────────────────────

struct DerivedTraits {
    float health_delta      = 0.0f;
    float energy_delta      = 0.0f;
    float speed_delta       = 0.0f;
    float mass_delta        = 0.0f;
    float size_delta        = 0.0f;   ///< Visible size change
    float r_delta           = 0.0f;   ///< RGB color deltas (VISIBLE)
    float g_delta           = 0.0f;
    float b_delta           = 0.0f;
    float aggression_delta  = 0.0f;
    float fear_delta        = 0.0f;
    float metabolism_delta  = 0.0f;
    float fertility_delta   = 0.0f;
    float iq_delta          = 0.0f;
    float lifespan_delta    = 0.0f;
    float resistance_delta  = 0.0f;
    float regen_delta       = 0.0f;
    // Secondaries computed from primaries
    float attack_power_delta= 0.0f;
    float defense_delta     = 0.0f;
    float sight_delta       = 0.0f;
};

/// Decode a 16-float genome into delta values.
/// Each gene is in [0,1]; we re-centre to [-1,+1] and scale.
[[nodiscard]] inline DerivedTraits decode(const Shape::CreatureGenome& g,
                                          const TraitWeights& w = {}) noexcept {
    const auto u = [](float x) noexcept { return x * 2.0f - 1.0f; };
    DerivedTraits d;

    d.health_delta     = u(g.genes[0])  * w.health_scale;
    d.energy_delta     = u(g.genes[1])  * w.energy_scale;
    d.speed_delta      = u(g.genes[2])  * w.speed_scale;
    d.mass_delta       = u(g.genes[3])  * w.mass_scale;
    d.size_delta       = u(g.genes[4])  * w.size_scale;

    // Color shift: three independent channels
    const float h = u(g.genes[5]) * w.hue_scale;
    const float s = u(g.genes[6]) * w.sat_scale;
    const float l = u(g.genes[7]) * w.light_scale;
    d.r_delta = std::clamp( h + l,            -1.0f, 1.0f);
    d.g_delta = std::clamp(-h * 0.5f + s + l, -1.0f, 1.0f);
    d.b_delta = std::clamp( h * 0.5f - s + l, -1.0f, 1.0f);

    d.aggression_delta = u(g.genes[8])  * w.aggression_scale;
    d.fear_delta       = u(g.genes[9])  * w.fear_scale;
    d.metabolism_delta = u(g.genes[10]) * w.metabolism_scale;
    d.fertility_delta  = u(g.genes[11]) * w.fertility_scale;
    d.iq_delta         = u(g.genes[12]) * w.iq_scale;
    d.lifespan_delta   = u(g.genes[13]) * w.lifespan_scale;
    d.resistance_delta = u(g.genes[14]) * w.resistance_scale;
    d.regen_delta      = u(g.genes[15]) * w.regen_scale;

    // Secondary
    d.attack_power_delta = d.size_delta * 0.5f + d.health_delta * 0.05f;
    d.defense_delta      = d.mass_delta * 2.0f + d.resistance_delta * 3.0f;
    d.sight_delta        = d.iq_delta   * 0.4f + d.speed_delta * 0.3f;

    return d;
}

// ─── CreatureStats — per-creature attribute cache ─────────────────────────────

/// Flattened snapshot of a creature's effective attributes, computed at spawn
/// and cached as a DerivedAttributes ECS2 component.
struct CreatureStats {
    float max_health        = 100.0f;
    float max_energy        = 100.0f;
    float max_speed         = 50.0f;
    float mass              = 1.0f;
    float size              = 7.0f;     ///< circumradius for rendering
    float aggression        = 0.30f;
    float fear              = 0.30f;
    float metabolism        = 0.50f;    ///< energy drain per tick
    float fertility         = 0.50f;
    float iq                = 50.0f;    ///< raw IQ points
    float lifespan          = 4800.0f;  ///< ticks
    float disease_resistance= 0.50f;
    float regen_rate        = 0.10f;
    float attack_power      = 10.0f;
    float defense           = 5.0f;
    float digestion_eff     = 0.70f;
    float sight_range       = 100.0f;
    float gestation_period  = 60.0f;    ///< ticks
    // RGB color (genome-blended)
    float color_r           = 1.0f;
    float color_g           = 1.0f;
    float color_b           = 1.0f;
};

/// Blend species baseline with genome deltas to produce a CreatureStats.
[[nodiscard]] inline CreatureStats apply_genome(const Shape::Species& sp,
                                                 const Shape::CreatureGenome& genome) noexcept {
    const DerivedTraits d = decode(genome);
    const auto c = [](float v, float lo, float hi) noexcept { return std::clamp(v, lo, hi); };

    CreatureStats s;
    s.max_health   = c(sp.biology.maxHealth        + d.health_delta,     10.0f, 2000.0f);
    s.max_energy   = c(sp.biology.maxEnergy        + d.energy_delta,     10.0f, 1000.0f);
    s.max_speed    = c(sp.physics.maxSpeed          + d.speed_delta,       5.0f,  400.0f);
    s.mass         = std::max(0.1f, sp.physics.mass + d.mass_delta);
    s.size         = std::max(1.0f, sp.geometry.circumradius + d.size_delta);
    s.aggression   = c(sp.psychology.aggression    + d.aggression_delta,  0.0f,   1.0f);
    s.fear         = c(sp.psychology.fear          + d.fear_delta,        0.0f,   1.0f);
    s.metabolism   = c(sp.biology.metabolismRate   + d.metabolism_delta,  0.05f,  3.0f);
    s.fertility    = c(sp.reproduction.fertilityBaseline + d.fertility_delta, 0.0f, 1.0f);
    s.iq           = c(sp.intelligence.rawIq        + d.iq_delta,         0.0f, 500.0f);
    s.lifespan     = std::max(600.0f, sp.biology.lifespan + d.lifespan_delta);
    s.disease_resistance = c(sp.biology.diseaseResistance + d.resistance_delta, 0.0f, 1.0f);
    s.regen_rate   = std::max(0.0f, sp.biology.regenerationRate + d.regen_delta);
    s.attack_power = std::max(1.0f, 10.0f + d.attack_power_delta + s.size * 0.5f);
    s.defense      = std::max(0.0f,  5.0f + d.defense_delta      + s.mass * 2.0f);
    s.digestion_eff= c(sp.biology.digestionEfficiency, 0.1f, 1.0f);
    s.sight_range  = std::max(20.0f, 100.0f + d.sight_delta);
    s.gestation_period = sp.reproduction.gestationPeriod;

    // Color: species primary color × genome shift
    const Color& base = sp.geometry.primaryColor;
    s.color_r = c(base.r + d.r_delta, 0.02f, 1.0f);
    s.color_g = c(base.g + d.g_delta, 0.02f, 1.0f);
    s.color_b = c(base.b + d.b_delta, 0.02f, 1.0f);

    return s;
}

// ─── Speciation helper ────────────────────────────────────────────────────────

/// Normalised L2 distance between two genomes in [0,1].
[[nodiscard]] inline float genomic_distance(const Shape::CreatureGenome& a,
                                             const Shape::CreatureGenome& b) noexcept {
    float sum = 0.0f;
    for (u8 i = 0; i < Shape::CreatureGenome::GENE_COUNT; ++i) {
        const float diff = a.genes[i] - b.genes[i];
        sum += diff * diff;
    }
    // sqrt(sum / N) normalised so max possible = 1.0
    return std::sqrt(sum / static_cast<float>(Shape::CreatureGenome::GENE_COUNT));
}

} // namespace Shape::Simulation::Genome
