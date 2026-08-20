import { afterEach, describe, expect, it } from "vitest";
import { DEFAULT_SETTINGS } from "@/sim/types";
import { Simulation } from "@/sim/simulation";
import { captureLocalSnapshot } from "@/sim/localSnapshot";
import { restoreSimulationSnapshot } from "@/sim/restoreSnapshot";

afterEach(() => {
  Reflect.deleteProperty(globalThis, "window");
  Reflect.deleteProperty(globalThis, "localStorage");
});

describe("snapshot -> restore integration", () => {
  it("restores deep containers and brain weights", () => {
    const sim = new Simulation(123, {
      ...DEFAULT_SETTINGS,
      initialPopulation: 20,
      initialFood: 20,
      neuralBrains: true,
    });

    for (let i = 0; i < 30; i += 1) sim.step();

    const snapshot = captureLocalSnapshot(sim);
    const restored = restoreSimulationSnapshot(snapshot);

    expect(restored.tick).toBe(sim.tick);
    expect(restored.organisms.length).toBe(sim.organisms.length);
    expect(restored.food.length).toBe(sim.food.length);
    expect(restored.species.length).toBe(sim.species.length);
    expect(restored.colonies.length).toBe(sim.colonies.length);
    expect(restored.structures.length).toBe(sim.structures.length);
    expect(restored.biomes.length).toBe(sim.biomes.length);
    expect(restored.remains.length).toBe(sim.remains.length);
    expect(restored.knowledgeNodes.length).toBe(sim.knowledgeNodes.length);

    const originalBrain = sim.organisms.find((o) => o.brain)?.brain;
    const restoredBrain = restored.organisms.find((o) => o.brain)?.brain;

    expect(restoredBrain?.weights.length).toBe(originalBrain?.weights.length);
  });
});
