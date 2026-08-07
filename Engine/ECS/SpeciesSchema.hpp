#pragma once

/// @file SpeciesSchema.hpp
/// @brief The 20-layer Species data structure for Polygonal Primordials.
///
/// A Species is a shared data template — all entities of the same species
/// reference one Species record via their speciesId. Individual runtime state
/// (current health, position, etc.) lives in per-entity ECS components.
///
/// DESIGN DOCTRINE: Emergence Over Scripting
///   - Every species uses the same 20-layer attribute schema (~250 floats).
///   - No special-case code per shape. Variation is purely numerical.
///   - Adding a new species requires zero engine changes — only a data entry.
///   - Two species with identical attribute values are functionally identical.

#include "Core/Platform.hpp"
#include "ShapeTaxonomy.hpp"
#include <array>
#include <span>

namespace Shape {

// ─── Forward declarations ─────────────────────────────────────────────────────
namespace Simulation { class DeterministicRng; }
using DeterministicRng = Simulation::DeterministicRng;

// ─── Constants ────────────────────────────────────────────────────────────────
static constexpr u16 MAX_KNOWN_TECHS      = 256;
static constexpr u16 MAX_CULTURAL_PRACTICES = 32;
static constexpr u16 GENOME_SIZE          = 64; ///< Genes per full species genome

using SpeciesId    = u16;
static constexpr SpeciesId INVALID_SPECIES_ID = 0;

// ─── Supporting types ─────────────────────────────────────────────────────────

/// A single gene in the full species genome.
struct Gene {
    f32 value      = 0.0f;
    f32 dominance  = 0.5f; ///< 0 = recessive, 1 = dominant
    u8  type       = 0;    ///< 0 = body, 1 = behavior, 2 = trait
};

/// Full species genome — used by the Evolution and Genetics systems.
/// Distinct from GeneticsComponent (which holds the smaller per-entity genome
/// used during early development).
struct SpeciesGenome {
    std::array<Gene, GENOME_SIZE> genes{};
    u8  count    = 0;
    u64 hash     = 0; ///< Fast identity comparison

    Gene&       operator[](u8 i)       noexcept { return genes[i]; }
    const Gene& operator[](u8 i) const noexcept { return genes[i]; }
};

/// A single cultural practice entry (religion, tradition, taboo, art form).
struct CulturalPractice {
    StringId nameId    = INVALID_STRING_ID;
    f32      strength  = 0.0f; ///< How deeply embedded (0–1)
    f32      spreadRate = 0.0f;
};

/// Lightweight religion doctrine descriptor.
struct ReligionDoctrine {
    StringId nameId              = INVALID_STRING_ID;
    f32      exclusivity         = 0.0f; ///< 0 = syncretic, 1 = exclusive
    f32      moralCodeStrength   = 0.0f;
    f32      afterlifeEmphasis   = 0.0f;
};

/// Resource inventory carried at the species or per-entity level.
/// Indexed by ResourceType (0–11).
struct Inventory {
    static constexpr u8 RESOURCE_COUNT = 12;
    std::array<f32, RESOURCE_COUNT> amounts{};
    f32&       operator[](u8 i)       noexcept { return amounts[i]; }
    const f32& operator[](u8 i) const noexcept { return amounts[i]; }
};

// ─── Layer 1: Geometry ────────────────────────────────────────────────────────
struct GeometryLayer {
    u16 vertexCount      = 3;     ///< 3 = triangle, 4 = quad, 64 = elite
    f32 circumradius     = 10.0f; ///< Body size
    f32 edgeLength       = 0.0f;  ///< Derived: 2*R*sin(π/n)
    f32 area             = 0.0f;  ///< Derived
    f32 perimeter        = 0.0f;  ///< Derived
    f32 interiorAngle    = 0.0f;  ///< (n-2)*180/n degrees
    f32 symmetryOrder    = 0.0f;  ///< n-fold rotational symmetry
    f32 turningRadius    = 1.0f;  ///< How tightly it can rotate (smaller = tighter)
    f32 visualComplexity = 0.0f;  ///< For rendering hint, not gameplay
    Color primaryColor   = Color::White();
    Color secondaryColor = Color::Black();
    f32 textureSeed      = 0.0f;
    f32 glowIntensity    = 0.0f;
    f32 trailStrength    = 0.0f;

    /// Recompute all derived fields from vertexCount and circumradius.
    void RecomputeDerived() noexcept;
};

// ─── Layer 2: Biology ─────────────────────────────────────────────────────────
struct BiologyLayer {
    f32 maxHealth               = 100.0f;
    f32 maxEnergy               = 100.0f;
    f32 metabolismRate          = 1.0f;  ///< Energy drain per tick
    f32 digestionEfficiency     = 0.7f;  ///< Food → Energy ratio
    f32 growthRate              = 1.0f;  ///< Juvenile → Adult speed
    f32 maturationPeriod        = 600.0f;///< Ticks
    f32 lifespan                = 4800.0f;///< Ticks (80 seconds at 60 Hz)
    f32 regenerationRate        = 0.1f;
    f32 diseaseResistance       = 0.5f;
    f32 immuneStrength          = 0.5f;
    f32 toxinResistance         = 0.5f;
    f32 radiationResistance     = 0.3f;
    f32 temperatureToleranceMin = -10.0f;
    f32 temperatureToleranceMax =  40.0f;
    f32 pressureTolerance       = 1.0f;
    f32 oxygenRequirement       = 1.0f;
    f32 waterRequirement        = 1.0f;
};

// ─── Layer 3: Intelligence ────────────────────────────────────────────────────
struct IntelligenceLayer {
    f32 rawIq               = 50.0f;
    f32 abstractReasoning   = 0.3f;
    f32 spatialReasoning    = 0.5f;
    f32 patternRecognition  = 0.4f;
    f32 memoryCapacity      = 5.0f;  ///< Short+long term slots
    f32 attentionSpan       = 0.5f;
    f32 problemSolvingSpeed = 0.5f;
    f32 learningRate        = 1.0f;  ///< Multiplier on experience gain
    f32 creativity          = 0.3f;  ///< Discovery / invention chance
    f32 languageAbility     = 0.3f;  ///< Communication complexity
    f32 planningDepth       = 2.0f;  ///< Steps ahead they can think
    f32 decisionQuality     = 0.5f;  ///< Reduces AI mistakes
};

// ─── Layer 4: Psychology ──────────────────────────────────────────────────────
struct PsychologyLayer {
    f32 aggression              = 0.5f; ///< 0 = passive, 1 = warlike
    f32 fear                    = 0.5f; ///< 0 = fearless, 1 = cowardly
    f32 curiosity               = 0.5f;
    f32 obedience               = 0.5f; ///< Tendency to follow leaders
    f32 ambition                = 0.5f;
    f32 greed                   = 0.3f;
    f32 empathy                 = 0.5f; ///< Will help non-kin
    f32 loyalty                 = 0.7f;
    f32 patience                = 0.5f;
    f32 impulsiveness           = 0.3f;
    f32 riskTolerance           = 0.5f;
    f32 happinessBaseline       = 0.6f;
    f32 stressResistance        = 0.5f;
    f32 depressionSusceptibility= 0.3f;
    f32 moraleBaseline          = 0.6f;
};

// ─── Layer 5: Society ─────────────────────────────────────────────────────────
struct SocietyLayer {
    f32 socialNeed              = 0.7f; ///< Drive to form groups
    f32 hierarchyAcceptance     = 0.5f; ///< 0 = egalitarian, 1 = feudal
    f32 collectivism            = 0.5f; ///< Group vs individual priority
    f32 familyBonding           = 0.8f;
    f32 inGroupFavoritism       = 0.6f; ///< Xenophobia strength
    f32 outGroupTolerance       = 0.4f;
    f32 matingStrategy          = 0.5f; ///< 0 = monogamy, 1 = polygamy
    f32 inheritanceRules        = 0.5f; ///< 0 = merit, 1 = bloodline
    f32 cooperationSkill        = 0.5f;
    f32 socialMemory            = 0.5f; ///< How long they remember slights
    f32 reputationWeight        = 0.5f;
    f32 leadershipResponsiveness= 0.6f;
};

// ─── Layer 6: Economy ─────────────────────────────────────────────────────────
struct EconomyLayer {
    f32 productionEfficiency    = 1.0f;
    f32 consumptionEfficiency   = 1.0f;
    f32 tradeSkill              = 0.5f;
    f32 wealthAccumulation      = 0.4f;
    f32 generosity              = 0.5f;
    f32 marketAwareness         = 0.3f;
    f32 resourceSpecialization  = 0.4f;
    f32 laborDiscipline         = 0.6f;
    f32 innovationInEconomy     = 0.3f;
    f32 corruptionTendency      = 0.2f;
    f32 taxCompliance           = 0.7f;
    f32 investmentWillingness   = 0.4f;
};

// ─── Layer 7: Military ────────────────────────────────────────────────────────
struct MilitaryLayer {
    f32 combatSkillBase         = 0.5f;
    f32 formationDiscipline     = 0.4f;
    f32 weaponMastery           = 0.4f;
    f32 tacticalSense           = 0.4f;
    f32 strategicSense          = 0.3f;
    f32 logisticsSkill          = 0.4f;
    f32 fortificationSkill      = 0.3f;
    f32 siegeSkill              = 0.2f;
    f32 navalSkill              = 0.2f;
    f32 airSkill                = 0.0f;
    f32 rangedCombat            = 0.4f;
    f32 meleeCombat             = 0.5f;
    f32 moraleBreakingThreshold = 0.3f;
    f32 berserkProbability      = 0.05f;
    f32 retreatThreshold        = 0.25f;
};

// ─── Layer 8: Construction ────────────────────────────────────────────────────
struct ConstructionLayer {
    f32 buildingSpeed           = 1.0f;
    f32 buildingQuality         = 0.5f; ///< Durability
    f32 architecturalSense      = 0.3f;
    f32 engineeringSkill        = 0.3f;
    f32 miningSkill             = 0.4f;
    f32 loggingSkill            = 0.4f;
    f32 farmingSkill            = 0.5f;
    f32 smithingSkill           = 0.3f;
    f32 masonrySkill            = 0.3f;
    f32 carpentrySkill          = 0.4f;
    f32 maxStructureComplexity  = 1.0f; ///< Tier they can build to
    f32 blueprintMemory         = 0.5f; ///< Can teach others
};

// ─── Layer 9: Diplomacy ───────────────────────────────────────────────────────
struct DiplomacyLayer {
    f32 negotiationSkill        = 0.5f;
    f32 trustworthiness         = 0.6f;
    f32 grudgeRetention         = 0.5f;
    f32 forgiveness             = 0.5f;
    f32 betrayalTendency        = 0.2f;
    f32 allianceAppreciation    = 0.6f;
    f32 culturalSensitivity     = 0.4f;
    f32 giftGivingTendency      = 0.4f;
    f32 promiseKeeping          = 0.7f;
    f32 treatyCompliance        = 0.7f;
    f32 reputationSensitivity   = 0.6f;
    f32 honorCodeStrength       = 0.5f;
};

// ─── Layer 10: Technology ─────────────────────────────────────────────────────
struct TechnologyLayer {
    f32 researchSpeed           = 1.0f;
    f32 experimentationTendency = 0.3f;
    f32 knowledgeRetention      = 0.7f; ///< Across generations
    f32 toolUseAbility          = 0.5f;
    f32 abstractScienceSense    = 0.3f;
    f32 appliedEngineering      = 0.4f;
    f32 theoreticalCapacity     = 0.3f;
    f32 disseminationSpeed      = 0.5f; ///< Spread within civ
    f32 preservationSkill       = 0.5f; ///< Survive disasters
    f32 currentTechTier         = 0.0f; ///< 0 = stone, 10 = space
    std::array<u16, MAX_KNOWN_TECHS> knownTechs{};
    u16 knownTechCount          = 0;
};

// ─── Layer 11: Genetics ───────────────────────────────────────────────────────
struct GeneticsLayer {
    SpeciesGenome genome{};
    f32 mutationRate            = 0.05f;
    f32 recombinationRate       = 0.5f;
    f32 inbreedingDepression    = 0.3f; ///< Penalty for low diversity
    f32 outbreedingAdvantage    = 0.1f;
    f32 epigeneticFlexibility   = 0.2f; ///< Adaptation within lifetime
    f32 geneticDiversity        = 0.5f; ///< Population-level metric
    f32 heritabilityStrength    = 0.7f; ///< 0 = learned, 1 = genetic
    f32 dominancePattern        = 0.5f;
    f32 polygenicScoreCount     = 8.0f; ///< How many traits are polygenic
};

// ─── Layer 12: Evolution ──────────────────────────────────────────────────────
struct EvolutionLayer {
    f32 selectionPressure           = 0.5f;
    f32 sexualSelectionStrength     = 0.4f;
    f32 nicheSpecialization         = 0.4f; ///< Tendency to specialize
    f32 adaptiveRadius              = 0.3f; ///< Genetic variance
    f32 speciationThreshold         = 0.7f; ///< When sub-populations diverge
    f32 extinctionResistance        = 0.5f;
    f32 fitnessLandscapePosition    = 0.5f;
    f32 evolutionaryMomentum        = 0.3f;
    f32 punctuatedEquilibriumTendency = 0.2f;
    f32 evolvability                = 0.4f; ///< Capacity to produce new traits
};

// ─── Layer 13: Physics ────────────────────────────────────────────────────────
/// Species-level physics properties (shared template for all members).
/// Per-entity physics state lives in RigidBodyComponent.
struct PhysicsLayer {
    f32 mass                    = 1.0f;
    f32 density                 = 1.0f;
    f32 hardness                = 0.5f; ///< Material resistance
    f32 elasticity              = 0.5f;
    f32 tensileStrength         = 0.5f;
    f32 frictionCoefficient     = 0.4f;
    f32 dragCoefficient         = 0.2f;
    f32 maxSpeed                = 50.0f;
    f32 acceleration            = 10.0f;
    f32 impactTolerance         = 0.5f;
    f32 thermalConductivity     = 0.3f;
    f32 electricalConductivity  = 0.1f;
    f32 magneticSusceptibility  = 0.0f;
    f32 reflectivity            = 0.3f;
};

// ─── Layer 14: Chemistry ──────────────────────────────────────────────────────
struct ChemistryLayer {
    f32 metabolicComplexity     = 0.3f;
    f32 toxinProduction         = 0.0f;
    f32 antitoxinProduction     = 0.3f;
    f32 acidResistance          = 0.4f;
    f32 baseResistance          = 0.4f;
    f32 oxidationRate           = 0.2f;
    f32 catalysisSkill          = 0.1f; ///< Industrial chemistry
    f32 compoundSynthesis       = 0.1f; ///< Can create new molecules
    f32 biochemistryEfficiency  = 0.5f;
    f32 pheromoneStrength       = 0.3f;
    f32 scentDetection          = 0.4f;
    f32 chemicalSignalRange     = 5.0f;
};

// ─── Layer 15: Culture ────────────────────────────────────────────────────────
struct CultureLayer {
    f32 artisticExpression      = 0.3f;
    f32 musicAbility            = 0.3f;
    f32 storytelling            = 0.4f;
    f32 symbolismCapacity       = 0.3f;
    f32 ritualTendency          = 0.4f;
    f32 traditionStrength       = 0.5f;
    f32 innovationInCulture     = 0.3f;
    f32 culturalMemory          = 0.5f;
    f32 culturalSpreadRate      = 0.2f;
    f32 culturalAbsorption      = 0.4f; ///< Can adopt others' culture
    f32 taboosCount             = 3.0f;
    f32 sacredValues            = 2.0f;
    std::array<CulturalPractice, MAX_CULTURAL_PRACTICES> practices{};
    u8  practiceCount           = 0;
};

// ─── Layer 16: Religion ───────────────────────────────────────────────────────
struct ReligionLayer {
    f32 spiritualTendency       = 0.4f;
    f32 beliefStrength          = 0.5f;
    f32 ritualComplexity        = 0.3f;
    f32 afterlifeBelief         = 0.5f;
    f32 deityCount              = 1.0f; ///< Polytheism spectrum
    f32 clergyInfluence         = 0.3f;
    f32 religiousTolerance      = 0.5f;
    f32 fanaticismTendency      = 0.2f;
    f32 syncretism              = 0.4f; ///< Can merge beliefs
    f32 missionaryZest          = 0.3f;
    f32 sacredTextKnowledge     = 0.2f;
    f32 miracleExpectation      = 0.3f;
    ReligionDoctrine doctrine{};
};

// ─── Layer 17: Leadership ─────────────────────────────────────────────────────
struct LeadershipLayer {
    f32 authorityNaturalness    = 0.5f; ///< Charisma
    f32 commandClarity          = 0.5f;
    f32 delegationSkill         = 0.4f;
    f32 decisionSpeed           = 0.5f;
    f32 decisionQuality         = 0.5f;
    f32 corruptionResistance    = 0.6f;
    f32 visionCapacity          = 0.4f;
    f32 inspiringSpeech         = 0.4f;
    f32 consensusBuilding       = 0.5f;
    f32 ruthlessness            = 0.3f;
    f32 popularityManagement    = 0.5f;
    f32 successionPlanning      = 0.3f;
    f32 crisisManagement        = 0.5f;
};

// ─── Layer 18: Resources ──────────────────────────────────────────────────────
struct ResourcesLayer {
    f32 carryingCapacitySelf    = 100.0f; ///< Max sustainable population
    f32 territoryNeed           = 10.0f;
    f32 resourceConservation    = 0.5f;
    f32 stockpileTendency       = 0.4f;
    f32 extractionEfficiency    = 0.5f;
    f32 refiningEfficiency      = 0.4f;
    f32 transportationSkill     = 0.4f;
    f32 storageSkill            = 0.5f;
    f32 recyclingTendency       = 0.2f;
    f32 discoveryAbility        = 0.3f; ///< Find new deposits
    Inventory inventory{};
};

// ─── Layer 19: Reproduction ───────────────────────────────────────────────────
struct ReproductionLayer {
    f32 fertilityBaseline       = 0.5f;
    f32 reproductiveAgeMin      = 120.0f; ///< Ticks
    f32 reproductiveAgeMax      = 3600.0f;///< Ticks
    f32 gestationPeriod         = 60.0f;  ///< Ticks
    f32 offspringCountMean      = 1.0f;
    f32 offspringCountVariance  = 0.5f;
    f32 parentalInvestment      = 0.7f;
    f32 matingDisplayComplexity = 0.4f;
    f32 mateSelectionSkill      = 0.5f;
    f32 reproductiveCostEnergy  = 30.0f;
    f32 reproductiveCostHealth  = 10.0f;
    f32 cloningCapability       = 0.0f; ///< Asexual reproduction ability
    f32 sexRatioSkew            = 0.5f; ///< 0.5 = 50/50
};

// ─── Layer 20: Civilization ───────────────────────────────────────────────────
struct CivilizationLayer {
    CivilizationTier tier                = CivilizationTier::Population;
    u32 currentPopulation                = 1;
    u32 peakPopulation                   = 1;
    f32 organizationalComplexity         = 0.0f;
    f32 recordKeepingSkill               = 0.0f;
    f32 bureaucracyEfficiency            = 0.0f;
    f32 lawComplexity                    = 0.0f;
    f32 enforcementStrength              = 0.0f;
    f32 corruptionLevel                  = 0.3f;
    f32 innovationCulture                = 0.3f;
    f32 longTermPlanning                 = 0.3f;
    f32 shortTermThinking                = 0.7f;
    f32 expansionDrive                   = 0.5f;
    f32 cohesionStrength                 = 0.5f;
    f32 civilizationalAge                = 0.0f;
    f32 declineResistance                = 0.5f;
    f32 renaissancePotential             = 0.3f;
};

// ─── Derived Stats ────────────────────────────────────────────────────────────
/// Computed from the 20 layers. Cached and updated on change.
struct SpeciesDerivedStats {
    f32 powerScore      = 0.0f; ///< Composite combat/influence metric
    f32 viabilityScore  = 0.0f; ///< Survival likelihood [0,1]
    f32 techLevel       = 0.0f; ///< Normalized 0–10
    f32 populationGrowthRate = 0.0f;
    f32 economicOutput  = 0.0f;
    f32 militaryStrength= 0.0f;
    ShapeTaxonomy taxonomy = ShapeTaxonomy::Primitive;
};

// ─── The Species Class ────────────────────────────────────────────────────────
/// The complete species definition. All 20 layers combined.
/// Shared across all entities of the same species via SpeciesId.
///
/// Instance storage: SpeciesRegistry (global resource in World).
/// Per-entity link: SpeciesIdComponent { SpeciesId id; }.
struct Species {
    SpeciesId id        = INVALID_SPECIES_ID;
    StringId  nameId    = INVALID_STRING_ID;

    // ── 20 Layers ────────────────────────────────────────────────────────────
    GeometryLayer      geometry{};
    BiologyLayer       biology{};
    IntelligenceLayer  intelligence{};
    PsychologyLayer    psychology{};
    SocietyLayer       society{};
    EconomyLayer       economy{};
    MilitaryLayer      military{};
    ConstructionLayer  construction{};
    DiplomacyLayer     diplomacy{};
    TechnologyLayer    technology{};
    GeneticsLayer      genetics{};
    EvolutionLayer     evolution{};
    PhysicsLayer       physics{};
    ChemistryLayer     chemistry{};
    CultureLayer       culture{};
    ReligionLayer      religion{};
    LeadershipLayer    leadership{};
    ResourcesLayer     resources{};
    ReproductionLayer  reproduction{};
    CivilizationLayer  civilization{};

    // ── Derived (computed) ────────────────────────────────────────────────────
    SpeciesDerivedStats derived{};

    // ── Classification ────────────────────────────────────────────────────────
    ShapeTaxonomy taxonomyTier = ShapeTaxonomy::Primitive;

    /// Recompute all derived stats and taxonomy from the 20 layers.
    void RecomputeDerived() noexcept;

    /// Composite power score: weighted sum of key attributes.
    f32 PowerScore() const noexcept;

    /// Viability score: likelihood of population survival (0–1).
    f32 ViabilityScore() const noexcept;
};

// ─── SpeciesIdComponent ───────────────────────────────────────────────────────
/// Lightweight ECS component linking an entity to its species template.
/// All entities of the same species share one Species record.
struct SpeciesIdComponent {
    SpeciesId id = INVALID_SPECIES_ID;
};

} // namespace Shape
