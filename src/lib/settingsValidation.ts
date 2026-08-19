/**
 * settingsValidation — clamps + bounds-check SimSettings
 *
 * The simulation accepts many numeric controls. Without validation,
 * a UI bug, corrupted localStorage, or copy-paste mistake could crash
 * the simulation or break invariants.
 *
 * License: MIT
 */

import { DEFAULT_SETTINGS, type BoundaryMode, type SimSettings } from "@/sim/types";

export type { BoundaryMode, SimSettings };
export { DEFAULT_SETTINGS };

const boundaryModes = new Set<BoundaryMode>(["wrap", "reflect", "open"]);

// Bounds (the simulation cannot tolerate values outside these)
export const SETTINGS_BOUNDS = {
  worldWidth: { min: 100, max: 20000, default: 800 },
  worldHeight: { min: 100, max: 12000, default: 600 },
  initialPopulation: { min: 1, max: 5000, default: 80 },
  maxPopulation: { min: 1, max: 1000000, default: 600 },
  initialFood: { min: 0, max: 10000, default: 200 },
  startingFood: { min: 0, max: 10000, default: 200 },
  foodGrowthRate: { min: 0, max: 100, default: 0.02 },
  mutationRate: { min: 0, max: 1, default: 0.05 },
  reproductionThreshold: { min: 0, max: 1000, default: 0.6 },
  maxFood: { min: 1, max: 1000000, default: 500 },
  colonyRadius: { min: 10, max: 1000, default: 80 },
  generationCycleTicks: { min: 30, max: 1000000, default: 300 },
  worldExpansionInterval: { min: 60, max: 1000000, default: 1200 },
  maxAge: { min: 10, max: 10000, default: 200 },
  generationInterval: { min: 1, max: 10000, default: 60 },
  simulationStepHz: { min: 1, max: 120, default: 30 },
} as const;

export function clamp(value: number, min: number, max: number): number {
  if (typeof value !== "number" || Number.isNaN(value) || !Number.isFinite(value)) return min;
  return Math.min(max, Math.max(min, value));
}

export function clampNumber(value: unknown, min: number, max: number, fallback: number): number {
  const n = typeof value === "number" && Number.isFinite(value) ? value : fallback;
  return Math.min(max, Math.max(min, n));
}

export function isPlainObject(v: unknown): v is Record<string, unknown> {
  return typeof v === "object" && v !== null && !Array.isArray(v);
}

/**
 * Validate + clamp a SimSettings object.
 *
 * Always returns a fully populated, in-bounds SimSettings.
 * Unknown keys are dropped. Invalid values fall back to defaults.
 */
export function validateSettings(
  input: unknown,
  defaults: SimSettings = DEFAULT_SETTINGS
): SimSettings {
  const out: SimSettings = { ...defaults };

  if (!isPlainObject(input)) return out;

  for (const [key, value] of Object.entries(input)) {
    if (!(key in out)) continue;

    const def = (out as Record<string, unknown>)[key];

    if (typeof def === "number") {
      if (typeof value !== "number" || Number.isNaN(value) || !Number.isFinite(value)) {
        continue;
      }
      const bounds = (SETTINGS_BOUNDS as Record<string, { min: number; max: number }>)[key];
      if (bounds) {
        (out as unknown as Record<string, number>)[key] = clamp(value, bounds.min, bounds.max);
      } else {
        (out as unknown as Record<string, number>)[key] = value;
      }
    } else if (typeof def === "boolean") {
      if (typeof value === "boolean") {
        (out as unknown as Record<string, boolean>)[key] = value;
      }
    } else if (key === "boundaryMode") {
      if (typeof value === "string" && boundaryModes.has(value as BoundaryMode)) {
        out.boundaryMode = value as BoundaryMode;
      }
    } else if (def === null) {
      if (value === null || (typeof value === "number" && Number.isFinite(value))) {
        (out as unknown as Record<string, unknown>)[key] = value;
      }
    }
  }

  // Cross-field invariants
  if (out.maxPopulation < out.initialPopulation) {
    out.maxPopulation = Math.max(out.initialPopulation, defaults.maxPopulation);
  }
  if (out.foodGrowthRate < 0) out.foodGrowthRate = 0;

  return out;
}
