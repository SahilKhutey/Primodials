import { describe, expect, it } from "vitest";
import { restoreSimulationSnapshot } from "@/sim/restoreSnapshot";
import { DEFAULT_SETTINGS } from "@/sim/types";
import type { LocalSimulationSnapshot } from "@/sim/localSnapshot";

describe("Compatibility — Corrupt V1 Snapshot Handling", () => {
  it("fills missing defaults when legacy v1 snapshot lacks newly introduced fields", () => {
    const sparseSnapshot = {
      version: 1,
      savedAt: "2026-01-01T00:00:00.000Z",
      tick: 20,
      organisms: [
        {
          id: 1,
          x: 50,
          y: 50,
          angle: 0,
          energy: 50,
          age: 0,
          generation: 1,
          genome: {
            speed: 1,
            size: 8,
            senseRadius: 40,
            foodAttraction: 1,
            dangerAvoidance: 1,
            socialDrive: 0,
            learningRate: 0.1,
            colorHue: 100,
            generation: 1,
            efficiency: 1,
          },
          speciesId: 1,
          brain: null,
          // note: threatLevel, buildCooldown, carrying, knowledgeBoost, etc. omitted intentionally
        }
      ],
      food: [],
      species: [],
      // colonies, structures, biomes omitted
      stats: {
        totalBorn: 1,
        totalDead: 0,
        maxGeneration: 1,
        generationCycle: 1,
        peakPopulation: 1,
        extinctions: 0,
        dominantSpeciesId: null,
      },
      settings: DEFAULT_SETTINGS,
    } as unknown as LocalSimulationSnapshot;

    const sim = restoreSimulationSnapshot(sparseSnapshot);
    const org = sim.organisms[0];

    expect(org.threatLevel).toBe(0);
    expect(org.buildCooldown).toBe(0);
    expect(org.carrying).toBe(0);
    expect(org.knowledgeBoost).toBe(0);
    expect(org.biomeId).toBe(0);
    expect(org.inBiofilm).toBe(false);
    expect(org.alive).toBe(true);
    expect(sim.colonies).toEqual([]);
    expect(sim.structures).toEqual([]);
  });
});
