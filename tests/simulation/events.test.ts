import { describe, expect, it } from "vitest";
import { DEFAULT_SETTINGS } from "@/sim/types";
import { Simulation } from "@/sim/simulation";

describe("simulation events", () => {
  it("adds food at a clamped coordinate", () => {
    const sim = new Simulation(1, {
      ...DEFAULT_SETTINGS,
      initialPopulation: 1,
      initialFood: 0,
    });

    sim.spawnFoodAt(-100, 99999, 10);

    const food = sim.food.at(-1);
    expect(food).toBeDefined();
    expect(food?.x).toBe(0);
    expect(food?.y).toBe(DEFAULT_SETTINGS.worldHeight);
    expect(food?.energy).toBeGreaterThanOrEqual(15);
  });

  it("meteor strike marks affected organisms dead and creates remains", () => {
    const sim = new Simulation(2, {
      ...DEFAULT_SETTINGS,
      initialPopulation: 5,
      initialFood: 0,
    });

    const target = sim.organisms[0];
    const beforeDeaths = sim.stats.deaths;
    const beforeRemains = sim.remains.length;

    sim.triggerMeteorStrike(target.x, target.y, 1000);

    expect(sim.stats.deaths).toBeGreaterThanOrEqual(beforeDeaths + 1);
    expect(sim.remains.length).toBeGreaterThan(beforeRemains);
    expect(target.alive).toBe(false);
  });
});
