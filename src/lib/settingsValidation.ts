import { DEFAULT_SETTINGS, type BoundaryMode, type SimSettings } from "@/sim/types";

const boundaryModes = new Set<BoundaryMode>(["wrap", "reflect", "open"]);

export function clampNumber(value: unknown, min: number, max: number, fallback: number) {
  const n = typeof value === "number" && Number.isFinite(value) ? value : fallback;
  return Math.min(max, Math.max(min, n));
}

export function validateSettings(input: Partial<SimSettings>, defaults: SimSettings = DEFAULT_SETTINGS): SimSettings {
  const inp = (input && typeof input === "object" ? input : {}) as Record<string, unknown>;
  const next: SimSettings = {
    worldWidth: Math.round(clampNumber(inp.worldWidth, 400, 20000, defaults.worldWidth)),
    worldHeight: Math.round(clampNumber(inp.worldHeight, 300, 12000, defaults.worldHeight)),
    initialPopulation: Math.round(clampNumber(inp.initialPopulation, 1, 5000, defaults.initialPopulation)),
    initialFood: Math.round(clampNumber(inp.initialFood, 0, 10000, defaults.initialFood)),
    foodGrowthRate: clampNumber(inp.foodGrowthRate, 0, 100, defaults.foodGrowthRate),
    mutationRate: clampNumber(inp.mutationRate, 0, 1, defaults.mutationRate),
    reproductionThreshold: clampNumber(inp.reproductionThreshold, 1, 1000, defaults.reproductionThreshold),
    maxPopulation: Math.round(clampNumber(inp.maxPopulation, 1, 1000000, defaults.maxPopulation)),
    maxFood: Math.round(clampNumber(inp.maxFood, 1, 1000000, defaults.maxFood)),
    colonyRadius: clampNumber(inp.colonyRadius, 10, 1000, defaults.colonyRadius),
    generationCycleTicks: Math.round(clampNumber(inp.generationCycleTicks, 30, 1000000, defaults.generationCycleTicks)),
    worldExpansionInterval: Math.round(
      clampNumber(inp.worldExpansionInterval, 60, 1000000, defaults.worldExpansionInterval),
    ),
    sexualReproduction: typeof inp.sexualReproduction === "boolean" ? inp.sexualReproduction : defaults.sexualReproduction,
    colonyFormation: typeof inp.colonyFormation === "boolean" ? inp.colonyFormation : defaults.colonyFormation,
    structureBuilding: typeof inp.structureBuilding === "boolean" ? inp.structureBuilding : defaults.structureBuilding,
    biomes: typeof inp.biomes === "boolean" ? inp.biomes : defaults.biomes,
    knowledgeNodes: typeof inp.knowledgeNodes === "boolean" ? inp.knowledgeNodes : defaults.knowledgeNodes,
    blueprints: typeof inp.blueprints === "boolean" ? inp.blueprints : defaults.blueprints,
    neuralBrains: typeof inp.neuralBrains === "boolean" ? inp.neuralBrains : defaults.neuralBrains,
    worldExpansion: typeof inp.worldExpansion === "boolean" ? inp.worldExpansion : defaults.worldExpansion,
    advancedBiology: typeof inp.advancedBiology === "boolean" ? inp.advancedBiology : defaults.advancedBiology,
    microbialBehavior: typeof inp.microbialBehavior === "boolean" ? inp.microbialBehavior : defaults.microbialBehavior,
    diseaseEvents: typeof inp.diseaseEvents === "boolean" ? inp.diseaseEvents : defaults.diseaseEvents,
    chemicalField: typeof inp.chemicalField === "boolean" ? inp.chemicalField : defaults.chemicalField,
    socialBehavior: typeof inp.socialBehavior === "boolean" ? inp.socialBehavior : defaults.socialBehavior,
    neutralDrift: typeof inp.neutralDrift === "boolean" ? inp.neutralDrift : defaults.neutralDrift,
    endlessGeneration: typeof inp.endlessGeneration === "boolean" ? inp.endlessGeneration : defaults.endlessGeneration,
    autoCheckpoint: typeof inp.autoCheckpoint === "boolean" ? inp.autoCheckpoint : defaults.autoCheckpoint,
    boundaryMode: boundaryModes.has(inp.boundaryMode as BoundaryMode) ? (inp.boundaryMode as BoundaryMode) : defaults.boundaryMode,
  };

  return next;
}
