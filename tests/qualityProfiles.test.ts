import { describe, expect, it } from "vitest";
import { DEFAULT_SETTINGS } from "../src/sim/types";
import { QUALITY_PROFILES, applyQualityProfile } from "../src/lib/qualityProfiles";

describe("quality profiles", () => {
  it("never increases population above the current setting", () => {
    const settings = { ...DEFAULT_SETTINGS, maxPopulation: 200 };
    const next = applyQualityProfile(settings, "high");
    expect(next.maxPopulation).toBe(200);
  });

  it("reduces heavy budgets for low quality", () => {
    const next = applyQualityProfile(DEFAULT_SETTINGS, "low");
    expect(next.maxPopulation).toBeLessThan(DEFAULT_SETTINGS.maxPopulation);
    expect(next.maxFood).toBeLessThan(DEFAULT_SETTINGS.maxFood);
  });

  it("has complete profiles", () => {
    expect(Object.keys(QUALITY_PROFILES).sort()).toEqual(["high", "low", "medium"]);
  });
});
