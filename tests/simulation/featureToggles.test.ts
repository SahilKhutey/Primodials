import { describe, expect, it } from "vitest";
import { DEFAULT_SETTINGS } from "@/sim/types";
import { Simulation } from "@/sim/simulation";

describe("simulation feature toggles", () => {
  it("initializes advanced biology systems without throwing", () => {
    const sim = new Simulation(5, {
      ...DEFAULT_SETTINGS,
      initialPopulation: 25,
      initialFood: 30,
      advancedBiology: true,
      microbialBehavior: true,
      diseaseEvents: true,
      chemicalField: true,
      socialBehavior: true,
      neutralDrift: true,
    });

    expect(sim.organisms.length).toBe(25);
    expect(sim.chemicalField).not.toBeNull();

    for (let i = 0; i < 180; i += 1) sim.step();

    expect(sim.tick).toBe(180);
  });
});
