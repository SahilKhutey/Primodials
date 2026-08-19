import type { BoundaryMode, SimSettings } from "@/sim/types";

const boundaryModes = new Set<BoundaryMode>(["wrap", "reflect", "open"]);

export function clampNumber(value: unknown, min: number, max: number, fallback: number) {
  const n = typeof value === "number" && Number.isFinite(value) ? value : fallback;
  return Math.min(max, Math.max(min, n));
}

export function validateSettings(input: Partial<SimSettings>, defaults: SimSettings): SimSettings {
  const next: SimSettings = { ...defaults, ...input };

  next.worldWidth = Math.round(clampNumber(next.worldWidth, 400, 20000, defaults.worldWidth));
  next.worldHeight = Math.round(clampNumber(next.worldHeight, 300, 12000, defaults.worldHeight));
  next.initialPopulation = Math.round(clampNumber(next.initialPopulation, 1, 5000, defaults.initialPopulation));
  next.initialFood = Math.round(clampNumber(next.initialFood, 0, 10000, defaults.initialFood));
  next.foodGrowthRate = clampNumber(next.foodGrowthRate, 0, 100, defaults.foodGrowthRate);
  next.mutationRate = clampNumber(next.mutationRate, 0, 1, defaults.mutationRate);
  next.reproductionThreshold = clampNumber(next.reproductionThreshold, 1, 1000, defaults.reproductionThreshold);
  next.maxPopulation = Math.round(clampNumber(next.maxPopulation, 1, 1000000, defaults.maxPopulation));
  next.maxFood = Math.round(clampNumber(next.maxFood, 1, 1000000, defaults.maxFood));
  next.colonyRadius = clampNumber(next.colonyRadius, 10, 1000, defaults.colonyRadius);
  next.generationCycleTicks = Math.round(clampNumber(next.generationCycleTicks, 30, 1000000, defaults.generationCycleTicks));
  next.worldExpansionInterval = Math.round(
    clampNumber(next.worldExpansionInterval, 60, 1000000, defaults.worldExpansionInterval),
  );

  if (!boundaryModes.has(next.boundaryMode)) next.boundaryMode = defaults.boundaryMode;
  return next;
}
