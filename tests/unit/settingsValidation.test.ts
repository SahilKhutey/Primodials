import { describe, expect, it } from "vitest";
import { DEFAULT_SETTINGS, type SimSettings } from "@/sim/types";
import { clampNumber, validateSettings } from "@/lib/settingsValidation";

describe("settings validation", () => {
  it("clamps every bounded numeric setting", () => {
    const invalid: Partial<SimSettings> = {
      worldWidth: -1,
      worldHeight: 999999,
      initialPopulation: 0,
      initialFood: 999999,
      foodGrowthRate: -5,
      mutationRate: 5,
      reproductionThreshold: 0,
      maxPopulation: 0,
      maxFood: -3,
      colonyRadius: 1,
      generationCycleTicks: 1,
      worldExpansionInterval: 1,
    };

    const result = validateSettings(invalid, DEFAULT_SETTINGS);

    expect(result.worldWidth).toBe(400);
    expect(result.worldHeight).toBe(12000);
    expect(result.initialPopulation).toBe(1);
    expect(result.initialFood).toBe(10000);
    expect(result.foodGrowthRate).toBe(0);
    expect(result.mutationRate).toBe(1);
    expect(result.reproductionThreshold).toBe(1);
    expect(result.maxPopulation).toBe(1);
    expect(result.maxFood).toBe(1);
    expect(result.colonyRadius).toBe(10);
    expect(result.generationCycleTicks).toBe(30);
    expect(result.worldExpansionInterval).toBe(60);
  });

  it("falls back to the default boundary mode", () => {
    const result = validateSettings(
      { boundaryMode: "invalid" as never },
      DEFAULT_SETTINGS,
    );
    expect(result.boundaryMode).toBe(DEFAULT_SETTINGS.boundaryMode);
  });

  it("preserves valid booleans and defaults", () => {
    const result = validateSettings(
      { neuralBrains: false, socialBehavior: false },
      DEFAULT_SETTINGS,
    );

    expect(result.neuralBrains).toBe(false);
    expect(result.socialBehavior).toBe(false);
    expect(result.worldWidth).toBe(DEFAULT_SETTINGS.worldWidth);
  });

  it("clampNumber returns fallback for non-finite input", () => {
    expect(clampNumber(Number.NaN, 0, 10, 5)).toBe(5);
    expect(clampNumber("abc", 0, 10, 3)).toBe(3);
  });
});
