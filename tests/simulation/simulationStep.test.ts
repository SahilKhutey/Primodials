import { describe, expect, it } from "vitest";
import { DEFAULT_SETTINGS } from "@/sim/types";
import { Simulation } from "@/sim/simulation";

function assertFiniteState(sim: Simulation) {
  for (const organism of sim.organisms) {
    for (const value of [
      organism.x,
      organism.y,
      organism.vx,
      organism.vy,
      organism.energy,
      organism.age,
      organism.fitness,
    ]) {
      expect(Number.isFinite(value)).toBe(true);
    }
  }

  for (const food of sim.food) {
    expect(Number.isFinite(food.x)).toBe(true);
    expect(Number.isFinite(food.y)).toBe(true);
    expect(Number.isFinite(food.energy)).toBe(true);
  }

  expect(Number.isFinite(sim.tick)).toBe(true);
}

describe("Simulation stepping", () => {
  it("advances ticks and preserves finite state", () => {
    const sim = new Simulation(111, {
      ...DEFAULT_SETTINGS,
      initialPopulation: 25,
      initialFood: 50,
      maxPopulation: 100,
      maxFood: 100,
    });

    for (let i = 0; i < 120; i += 1) {
      sim.step();
      assertFiniteState(sim);
    }

    expect(sim.tick).toBe(120);
    expect(sim.history.length).toBeGreaterThanOrEqual(0);
  });

  it("does not exceed twice the configured population ceiling during short soak", () => {
    const maxPopulation = 60;
    const sim = new Simulation(22, {
      ...DEFAULT_SETTINGS,
      initialPopulation: 20,
      initialFood: 30,
      maxPopulation,
      maxFood: 60,
    });

    for (let i = 0; i < 300; i += 1) sim.step();

    expect(sim.population).toBeLessThanOrEqual(maxPopulation * 2);
  });
});
