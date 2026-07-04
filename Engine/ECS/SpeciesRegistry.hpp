#pragma once

/// @file SpeciesRegistry.hpp
/// @brief Global registry of all Species definitions.
///
/// The SpeciesRegistry is a World resource (singleton) that owns all Species
/// records. Entities link to their species via SpeciesIdComponent { SpeciesId }.
/// This avoids duplicating ~250 floats per entity.
///
/// Usage:
///   auto& registry = world.GetResource<SpeciesRegistry>();
///   const Species& tri = registry.Get(triangleId);

#include "SpeciesSchema.hpp"
#include <array>
#include <optional>

namespace Shape {

static constexpr u16 MAX_SPECIES = 1024;

class SpeciesRegistry {
public:
    SpeciesRegistry() noexcept { m_Records.fill({}); }

    // ── Registration ─────────────────────────────────────────────────────────

    /// Register a species. Returns its assigned SpeciesId.
    /// Returns INVALID_SPECIES_ID if the registry is full.
    SpeciesId Register(Species species) noexcept {
        if (m_Count >= MAX_SPECIES) return INVALID_SPECIES_ID;
        SpeciesId id = static_cast<SpeciesId>(m_Count + 1); // IDs start at 1
        species.id   = id;
        species.taxonomyTier = ClassifyTaxonomy(species.geometry.vertexCount);
        species.RecomputeDerived();
        m_Records[m_Count] = species;
        ++m_Count;
        return id;
    }

    // ── Lookup ────────────────────────────────────────────────────────────────

    /// Returns the Species for a given id. Crashes in debug if id is invalid.
    SHAPE_NODISCARD const Species& Get(SpeciesId id) const noexcept {
        // IDs are 1-based; index = id - 1
        return m_Records[id - 1];
    }

    SHAPE_NODISCARD Species& Get(SpeciesId id) noexcept {
        return m_Records[id - 1];
    }

    SHAPE_NODISCARD bool IsValid(SpeciesId id) const noexcept {
        return id != INVALID_SPECIES_ID && id <= m_Count;
    }

    // ── Iteration ─────────────────────────────────────────────────────────────

    SHAPE_NODISCARD u16 Count() const noexcept { return m_Count; }

    /// Iterate over all registered species (deterministic insertion order).
    SHAPE_NODISCARD const Species* begin() const noexcept { return m_Records.data(); }
    SHAPE_NODISCARD const Species* end()   const noexcept { return m_Records.data() + m_Count; }
    SHAPE_NODISCARD Species*       begin()       noexcept { return m_Records.data(); }
    SHAPE_NODISCARD Species*       end()         noexcept { return m_Records.data() + m_Count; }

    // ── Lookup by vertex count ────────────────────────────────────────────────

    /// Find the first registered species with the given vertex count.
    /// Returns nullptr if none found.
    SHAPE_NODISCARD const Species* FindByVertexCount(u16 vertexCount) const noexcept {
        for (u16 i = 0; i < m_Count; ++i) {
            if (m_Records[i].geometry.vertexCount == vertexCount) {
                return &m_Records[i];
            }
        }
        return nullptr;
    }

    // ── Stats ─────────────────────────────────────────────────────────────────

    void Clear() noexcept {
        m_Records.fill({});
        m_Count = 0;
    }

private:
    std::array<Species, MAX_SPECIES> m_Records{};
    u16 m_Count = 0;
};

} // namespace Shape
