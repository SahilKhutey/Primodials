import { describe, expect, it } from "vitest";
import { restoreSimulationSnapshot } from "@/sim/restoreSnapshot";
import { DEFAULT_SETTINGS } from "@/sim/types";
import type { LocalSimulationSnapshot } from "@/sim/localSnapshot";

describe("Compatibility — V1 Snapshot Schema Contract", () => {
  it("deserializes a canonical v1 snapshot without loss", () => {
    const v1Snapshot: LocalSimulationSnapshot = {
      version: 1,
      savedAt: "2026-08-20T12:00:00.000Z",
      tick: 350,
      organisms: [
        {
          id: 101,
          x: 450.5,
          y: 280.2,
          angle: 1.57,
          energy: 120.0,
          age: 45,
          generation: 3,
          genome: {
            speed: 1.2,
            size: 9.5,
            senseRadius: 65,
            foodAttraction: 1.5,
            dangerAvoidance: 0.8,
            socialDrive: 0.4,
            learningRate: 0.08,
            colorHue: 210,
            generation: 3,
            efficiency: 1.1,
          },
          speciesId: 5,
          brain: {
            nInputs: 8,
            nHidden: 6,
            nOutputs: 4,
            weights: [0.1, -0.2, 0.5, 0.9, -0.4, 0.3],
          },
        } as unknown as LocalSimulationSnapshot["organisms"][0]
      ],
      food: [
        { id: 1, x: 200, y: 300, energy: 50, type: "plant" }
      ],
      species: [
        {
          id: 5,
          name: "Primordium Alpha",
          color: "hsl(210, 80%, 50%)",
          count: 1,
          representative: {
            speed: 1.2,
            size: 9.5,
            senseRadius: 65,
            foodAttraction: 1.5,
            dangerAvoidance: 0.8,
            socialDrive: 0.4,
            learningRate: 0.08,
            colorHue: 210,
            generation: 3,
            efficiency: 1.1,
          },
        } as unknown as LocalSimulationSnapshot["species"][0]
      ],
      colonies: [],
      structures: [],
      biomes: [],
      remains: [],
      knowledgeNodes: [],
      stats: {
        totalBorn: 15,
        totalDead: 14,
        maxGeneration: 3,
        generationCycle: 2,
        peakPopulation: 10,
        extinctions: 0,
        dominantSpeciesId: 5,
      },
      settings: DEFAULT_SETTINGS,
    };

    const sim = restoreSimulationSnapshot(v1Snapshot);

    expect(sim.tick).toBe(350);
    expect(sim.organisms).toHaveLength(1);
    expect(sim.organisms[0].id).toBe(101);
    expect(sim.organisms[0].x).toBeCloseTo(450.5);
    expect(sim.organisms[0].speciesId).toBe(5);
    expect(sim.organisms[0].brain).not.toBeNull();
    expect(sim.organisms[0].brain?.weights).toBeInstanceOf(Float32Array);
    expect(sim.species).toHaveLength(1);
    expect(sim.species[0].name).toBe("Primordium Alpha");
  });
});
