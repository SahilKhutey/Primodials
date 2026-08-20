import { describe, expect, it } from "vitest";
import { DEFAULT_SETTINGS } from "@/sim/types";
import { QUALITY_PROFILES, applyQualityProfile } from "@/lib/qualityProfiles";

describe("quality profiles", () => {
  it("contains all three profiles", () => {
    expect(Object.keys(QUALITY_PROFILES).sort()).toEqual(["high", "low", "medium"]);
  });

  it("never raises a user's explicit population ceiling", () => {
    const settings = { ...DEFAULT_SETTINGS, maxPopulation: 150 };
    expect(applyQualityProfile(settings, "high").maxPopulation).toBe(150);
  });

  it("low quality reduces budgets", () => {
    const low = applyQualityProfile(DEFAULT_SETTINGS, "low");
    expect(low.maxPopulation).toBeLessThan(DEFAULT_SETTINGS.maxPopulation);
    expect(low.maxFood).toBeLessThan(DEFAULT_SETTINGS.maxFood);
  });
});
