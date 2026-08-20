import { describe, expect, it } from "vitest";
import { restoreSimulationSnapshot } from "@/sim/restoreSnapshot";
import { DEFAULT_SETTINGS } from "@/sim/types";
import type { LocalSimulationSnapshot } from "@/sim/localSnapshot";

describe("Fault Injection — Snapshot Restoral", () => {
  it("survives snapshot with empty or null collections", () => {
    const minimalSnapshot = {
      version: 1,
      savedAt: new Date().toISOString(),
      tick: 100,
      organisms: [],
      food: [],
      species: [],
      colonies: [],
      structures: [],
      biomes: [],
      remains: [],
      knowledgeNodes: [],
      stats: {
        totalBorn: 0,
        totalDead: 0,
        maxGeneration: 1,
        generationCycle: 1,
        peakPopulation: 0,
        extinctions: 0,
        dominantSpeciesId: null,
      },
      settings: DEFAULT_SETTINGS,
    } as unknown as LocalSimulationSnapshot;

    expect(() => restoreSimulationSnapshot(minimalSnapshot)).not.toThrow();
    const sim = restoreSimulationSnapshot(minimalSnapshot);
    expect(sim.tick).toBe(100);
    expect(sim.population).toBe(0);
  });

  it("safely restores organism with missing or null brain", () => {
    const snapshotWithNullBrain = {
      version: 1,
      savedAt: new Date().toISOString(),
      tick: 50,
      organisms: [
        {
          id: 1,
          x: 100,
          y: 100,
          angle: 0,
          energy: 100,
          age: 10,
          generation: 1,
          genome: {
            speed: 1,
            size: 8,
            senseRadius: 50,
            foodAttraction: 1,
            dangerAvoidance: 1,
            socialDrive: 0,
            learningRate: 0.1,
            colorHue: 180,
            generation: 1,
            efficiency: 1,
          },
          speciesId: 1,
          brain: null,
        }
      ],
      food: [],
      species: [],
      colonies: [],
      structures: [],
      biomes: [],
      remains: [],
      knowledgeNodes: [],
      stats: {
        totalBorn: 1,
        totalDead: 0,
        maxGeneration: 1,
        generationCycle: 1,
        peakPopulation: 1,
        extinctions: 0,
        dominantSpeciesId: 1,
      },
      settings: DEFAULT_SETTINGS,
    } as unknown as LocalSimulationSnapshot;

    const sim = restoreSimulationSnapshot(snapshotWithNullBrain);
    expect(sim.organisms).toHaveLength(1);
    expect(sim.organisms[0].brain).toBeNull();
    expect(sim.organisms[0].alive).toBe(true);
  });
});
