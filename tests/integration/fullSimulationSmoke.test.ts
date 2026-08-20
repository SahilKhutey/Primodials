import { describe, expect, it } from "vitest";
import { DEFAULT_SETTINGS } from "@/sim/types";
import { Simulation } from "@/sim/simulation";
import { captureLocalSnapshot } from "@/sim/localSnapshot";
import { restoreSimulationSnapshot } from "@/sim/restoreSnapshot";
import { createWorldShare, decodeWorldShare } from "@/lib/worldShare";
import { applyQualityProfile } from "@/lib/qualityProfiles";

describe("full Phase 1–6 simulation smoke", () => {
  it("runs product-critical systems together", () => {
    const base = {
      ...DEFAULT_SETTINGS,
      initialPopulation: 25,
      initialFood: 40,
      maxPopulation: 100,
      maxFood: 100,
    };

    const wallpaperSettings = applyQualityProfile(base, "medium");
    const seed = 20260820;

    const sim = new Simulation(seed, wallpaperSettings);

    for (let i = 0; i < 300; i += 1) sim.step();

    expect(sim.tick).toBe(300);
    expect(sim.organisms.length).toBeGreaterThanOrEqual(0);

    const snapshot = captureLocalSnapshot(sim);
    const restored = restoreSimulationSnapshot(snapshot);

    expect(restored.tick).toBe(sim.tick);
    expect(restored.organisms.length).toBe(sim.organisms.length);

    const code = createWorldShare(seed, {
      worldWidth: wallpaperSettings.worldWidth,
      worldHeight: wallpaperSettings.worldHeight,
      maxPopulation: wallpaperSettings.maxPopulation,
      mutationRate: wallpaperSettings.mutationRate,
    });

    const world = decodeWorldShare(code);
    expect(world?.seed).toBe(seed);
  });
});
