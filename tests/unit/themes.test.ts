import { describe, expect, it } from "vitest";
import { DEFAULT_THEME_ID, PACING_PRESETS, THEMES, getTheme, tintHue } from "@/sim/themes";

describe("themes", () => {
  it("contains the default theme", () => {
    expect(THEMES.some((theme) => theme.id === DEFAULT_THEME_ID)).toBe(true);
    expect(getTheme(DEFAULT_THEME_ID).displayName.length).toBeGreaterThan(0);
  });

  it("all themes have valid bounded presentation values", () => {
    for (const theme of THEMES) {
      expect(theme.vignetteStrength).toBeGreaterThanOrEqual(0);
      expect(theme.vignetteStrength).toBeLessThanOrEqual(1);
      expect(theme.glowIntensity).toBeGreaterThan(0);
      expect(theme.creaturePalette.length).toBeGreaterThan(0);
      expect(theme.particleCount).toBeGreaterThanOrEqual(0);
    }
  });

  it("pacing presets have positive configuration values", () => {
    for (const preset of Object.values(PACING_PRESETS)) {
      expect(preset.speedMultiplier).toBeGreaterThan(0);
      expect(preset.maxPopulationMod).toBeGreaterThan(0);
      expect(preset.foodRateMod).toBeGreaterThan(0);
      expect(preset.mutationMod).toBeGreaterThan(0);
    }
  });

  it("tints hue to a bounded hue", () => {
    const theme = getTheme(DEFAULT_THEME_ID);
    const value = tintHue(359, theme);
    expect(value).toBeGreaterThanOrEqual(0);
    expect(value).toBeLessThan(360);
  });
});
