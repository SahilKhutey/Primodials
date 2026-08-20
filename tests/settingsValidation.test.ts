import { describe, expect, it } from "vitest";
import { DEFAULT_SETTINGS } from "../src/sim/types";
import { validateSettings } from "../src/lib/settingsValidation";

describe("validateSettings", () => {
  it("clamps invalid numeric values", () => {
    const result = validateSettings(
      { worldWidth: -100, mutationRate: 20, maxPopulation: 0 },
      DEFAULT_SETTINGS,
    );

    expect(result.worldWidth).toBe(400);
    expect(result.mutationRate).toBe(1);
    expect(result.maxPopulation).toBe(1);
  });

  it("falls back to defaults for invalid boundary modes", () => {
    const result = validateSettings(
      { boundaryMode: "invalid" as never },
      DEFAULT_SETTINGS,
    );

    expect(result.boundaryMode).toBe(DEFAULT_SETTINGS.boundaryMode);
  });
});
