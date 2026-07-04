#pragma once

/// @file ShapeTaxonomy.hpp
/// @brief Visual/niche tier classification for polygon species, and the
///        civilization progression tier system.
///
/// IMPORTANT: ShapeTaxonomy is NOT a power hierarchy. It defines starting
/// parameters and physical tradeoffs (size vs agility) only. A Triangle civ
/// with high tech and high population can dominate a Circle civ with low values.

#include "Core/Platform.hpp"

namespace Shape {

// ─── Shape Taxonomy ──────────────────────────────────────────────────────────
// Classifies species by vertex count into visual/evolutionary tiers.
// Two species with identical attribute values are functionally identical
// regardless of their taxonomy tier.

enum class ShapeTaxonomy : u8 {
    Anomalous,        ///< Monogon, Digon, Sentinel Circle — edge cases
    Primitive,        ///< Triangle (3), Quad (4), Pentagon (5)
    Developing,       ///< Hexagon (6) through Octagon (8)
    Advanced,         ///< 9-gon through 12-gon
    Elite,            ///< 13-gon through 64-gon
    Transcendent,     ///< Near-circle and Circle Civilization
    COUNT
};

/// @returns ShapeTaxonomy tier based on vertex count.
inline ShapeTaxonomy ClassifyTaxonomy(u16 vertexCount) noexcept {
    if (vertexCount <= 2)  return ShapeTaxonomy::Anomalous;
    if (vertexCount <= 5)  return ShapeTaxonomy::Primitive;
    if (vertexCount <= 8)  return ShapeTaxonomy::Developing;
    if (vertexCount <= 12) return ShapeTaxonomy::Advanced;
    if (vertexCount <= 64) return ShapeTaxonomy::Elite;
    return ShapeTaxonomy::Transcendent;
}

inline const char* TaxonomyName(ShapeTaxonomy t) noexcept {
    switch (t) {
        case ShapeTaxonomy::Anomalous:    return "Anomalous";
        case ShapeTaxonomy::Primitive:    return "Primitive";
        case ShapeTaxonomy::Developing:   return "Developing";
        case ShapeTaxonomy::Advanced:     return "Advanced";
        case ShapeTaxonomy::Elite:        return "Elite";
        case ShapeTaxonomy::Transcendent: return "Transcendent";
        default:                          return "Unknown";
    }
}

// ─── Civilization Tier ────────────────────────────────────────────────────────
// Emergent organizational tier. A "Kingdom" is not a class — it is a faction
// that meets specific population + infrastructure conditions (data-driven rules).

enum class CivilizationTier : u8 {
    Population,    ///< 1 - 9 individuals
    Family,        ///< 2 - 20 individuals
    Clan,          ///< 10 - 200 individuals
    Village,       ///< 100 - 2,000 individuals
    Town,          ///< 1,000 - 20,000 individuals
    City,          ///< 10,000 - 200,000 individuals
    Kingdom,       ///< 100,000 - 2,000,000 individuals
    Empire,        ///< 1,000,000 - 20,000,000 individuals
    Federation,    ///< Multi-faction alliance > 20,000,000
    Hegemony,      ///< Dominant single power
    Transcendent,  ///< Post-scarcity, post-biological
    COUNT
};

inline const char* CivilizationTierName(CivilizationTier t) noexcept {
    switch (t) {
        case CivilizationTier::Population:   return "Population";
        case CivilizationTier::Family:       return "Family";
        case CivilizationTier::Clan:         return "Clan";
        case CivilizationTier::Village:      return "Village";
        case CivilizationTier::Town:         return "Town";
        case CivilizationTier::City:         return "City";
        case CivilizationTier::Kingdom:      return "Kingdom";
        case CivilizationTier::Empire:       return "Empire";
        case CivilizationTier::Federation:   return "Federation";
        case CivilizationTier::Hegemony:     return "Hegemony";
        case CivilizationTier::Transcendent: return "Transcendent";
        default:                             return "Unknown";
    }
}

/// Evaluate the civilization tier purely from population count.
/// Full evaluation (requiring tech, infrastructure, org) is handled by
/// CivilizationSystem using data-driven TierRule tables.
inline CivilizationTier EvaluateCivilizationTierByPop(u32 population) noexcept {
    if (population >= 20'000'000) return CivilizationTier::Federation;
    if (population >=  1'000'000) return CivilizationTier::Empire;
    if (population >=    100'000) return CivilizationTier::Kingdom;
    if (population >=     10'000) return CivilizationTier::City;
    if (population >=      1'000) return CivilizationTier::Town;
    if (population >=        100) return CivilizationTier::Village;
    if (population >=         10) return CivilizationTier::Clan;
    if (population >=          2) return CivilizationTier::Family;
    return CivilizationTier::Population;
}

} // namespace Shape
