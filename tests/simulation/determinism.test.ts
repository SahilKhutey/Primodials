import { describe, expect, it } from "vitest";
import { DEFAULT_SETTINGS } from "@/sim/types";
import { Simulation } from "@/sim/simulation";

function digest(sim: Simulation) {
  return {
    tick: sim.tick,
    organisms: sim.organisms.slice(0, 25).map((o) => ({
      id: o.id,
      x: Number(o.x.toFixed(5)),
      y: Number(o.y.toFixed(5)),
      energy: Number(o.energy.toFixed(5)),
      age: o.age,
      generation: o.generation,
      speciesId: o.speciesId,
    })),
    food: sim.food.slice(0, 25).map((f) => ({
      id: f.id,
      x: Number(f.x.toFixed(5)),
      y: Number(f.y.toFixed(5)),
      energy: Number(f.energy.toFixed(5)),
    })),
  };
}

describe("simulation determinism", () => {
  it("produces the same initial world for the same seed", () => {
    const settings = {
      ...DEFAULT_SETTINGS,
      initialPopulation: 20,
      initialFood: 30,
    };

    const a = new Simulation(777, settings);
    const b = new Simulation(777, settings);

    expect(digest(a)).toEqual(digest(b));
  });

  it("produces repeatable short evolution for the same seed", () => {
    const settings = {
      ...DEFAULT_SETTINGS,
      initialPopulation: 15,
      initialFood: 25,
      maxPopulation: 50,
      maxFood: 60,
    };

    const a = new Simulation(314159, settings);
    const b = new Simulation(314159, settings);

    for (let i = 0; i < 90; i += 1) {
      a.step();
      b.step();
    }

    expect(digest(a)).toEqual(digest(b));
  });
});
