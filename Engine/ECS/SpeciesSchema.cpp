#include "ECS/SpeciesSchema.hpp"
#include "Math/MathUtils.hpp"

namespace Shape {

// ─── GeometryLayer ───────────────────────────────────────────────────────────

void GeometryLayer::RecomputeDerived() noexcept {
    using namespace Math;

    // Edge length: 2R * sin(π / n)
    f32 n = static_cast<f32>(vertexCount);
    edgeLength    = 2.0f * circumradius * Sin(PI / n);

    // Regular polygon area: (n * s^2) / (4 * tan(π/n))
    area = (n * edgeLength * edgeLength) / (4.0f * Math::Tan(PI / n));

    perimeter     = n * edgeLength;
    interiorAngle = (n - 2.0f) * 180.0f / n;
    symmetryOrder = n;

    // Larger polygons turn slower (more sides → more inertia)
    // Turning radius: 1 / n, normalized so Triangle = 1.0, Circle ≈ 0.05
    turningRadius = 1.0f / Math::Clamp(n / 3.0f, 1.0f, 20.0f);

    // Visual complexity: normalized to [0,1], maxes out at ~64 sides
    visualComplexity = Math::Clamp((n - 3.0f) / 61.0f, 0.0f, 1.0f);
}

// ─── Species ─────────────────────────────────────────────────────────────────

void Species::RecomputeDerived() noexcept {
    geometry.RecomputeDerived();
    taxonomyTier = ClassifyTaxonomy(geometry.vertexCount);
    derived.taxonomy = taxonomyTier;

    // Power score: weighted composite of key attributes
    // Weights reflect simulation design: combat, intelligence, pop capacity matter most.
    derived.powerScore =
        military.combatSkillBase        * 2.0f +
        intelligence.rawIq              * 0.02f +
        biology.maxHealth               * 0.01f +
        technology.currentTechTier      * 3.0f +
        civilization.cohesionStrength   * 1.5f +
        economy.productionEfficiency    * 1.0f;

    // Viability: survival likelihood (0–1)
    // Lower is better for aggression (peaceful species survive if defended)
    derived.viabilityScore = Math::Clamp(
        (biology.regenerationRate         * 2.0f +
         biology.diseaseResistance        * 1.0f +
         evolution.extinctionResistance   * 3.0f +
         reproduction.fertilityBaseline   * 2.0f +
         genetics.geneticDiversity        * 1.0f +
         (1.0f - evolution.selectionPressure) * 0.5f) / 10.0f,
        0.0f, 1.0f);

    derived.techLevel           = technology.currentTechTier;
    derived.economicOutput      = economy.productionEfficiency * economy.laborDiscipline;
    derived.militaryStrength    = military.combatSkillBase * military.formationDiscipline;
    derived.populationGrowthRate =
        Math::Clamp(reproduction.fertilityBaseline - biology.metabolismRate * 0.1f,
                    0.0f, 1.0f);
}

f32 Species::PowerScore() const noexcept {
    return derived.powerScore;
}

f32 Species::ViabilityScore() const noexcept {
    return derived.viabilityScore;
}

} // namespace Shape
