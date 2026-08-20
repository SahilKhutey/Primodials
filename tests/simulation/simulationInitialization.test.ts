import { describe, expect, it } from "vitest";
import { DEFAULT_SETTINGS } from "@/sim/types";
import { Simulation } from "@/sim/simulation";

describe("Simulation initialization", () => {
  it("initializes to configured population and food", () => {
    const settings = {
      ...DEFAULT_SETTINGS,
      initialPopulation: 18,
      initialFood: 24,
      neuralBrains: true,
      biomes: true,
      knowledgeNodes: true,
    };

    const sim = new Simulation(12345, settings);

    expect(sim.organisms).toHaveLength(18);
    expect(sim.food).toHaveLength(24);
    expect(sim.tick).toBe(0);
    expect(sim.stats.maxGeneration).toBe(0);
    expect(sim.species.length).toBeGreaterThan(0);
    expect(sim.biomes.length).toBeGreaterThan(0);
    expect(sim.knowledgeNodes.length).toBeGreaterThan(0);
  });

  it("disables optional subsystems when configured off", () => {
    const settings = {
      ...DEFAULT_SETTINGS,
      initialPopulation: 5,
      initialFood: 5,
      neuralBrains: false,
      biomes: false,
      knowledgeNodes: false,
      chemicalField: false,
    };

    const sim = new Simulation(1, settings);

    expect(sim.organisms.every((organism) => organism.brain === null)).toBe(true);
    expect(sim.biomes).toHaveLength(0);
    expect(sim.knowledgeNodes).toHaveLength(0);
    expect(sim.chemicalField).toBeNull();
  });
});
