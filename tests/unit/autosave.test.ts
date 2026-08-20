import { afterEach, describe, expect, it } from "vitest";
import { DEFAULT_SETTINGS } from "@/sim/types";
import { Simulation } from "@/sim/simulation";
import { captureLocalSnapshot, clearLocalSnapshot, loadLocalSnapshot, saveLocalSnapshot } from "@/sim/localSnapshot";
import { installMemoryBrowser } from "../fixtures/testStorage";

afterEach(() => {
  Reflect.deleteProperty(globalThis, "window");
  Reflect.deleteProperty(globalThis, "localStorage");
});

describe("local simulation snapshots", () => {
  it("captures the important simulation containers", () => {
    const sim = new Simulation(123, {
      ...DEFAULT_SETTINGS,
      initialPopulation: 12,
      initialFood: 20,
    });

    const snapshot = captureLocalSnapshot(sim);

    expect(snapshot.version).toBe(1);
    expect(snapshot.tick).toBe(sim.tick);
    expect(snapshot.organisms).toHaveLength(sim.organisms.length);
    expect(snapshot.food).toHaveLength(sim.food.length);
    expect(snapshot.species).toHaveLength(sim.species.length);
    expect(snapshot.settings.worldWidth).toBe(sim.settings.worldWidth);
  });

  it("saves and loads through local storage", () => {
    installMemoryBrowser();

    const sim = new Simulation(123, {
      ...DEFAULT_SETTINGS,
      initialPopulation: 10,
      initialFood: 10,
    });

    sim.step();
    expect(saveLocalSnapshot(sim)).toBe(true);

    const loaded = loadLocalSnapshot();
    expect(loaded).not.toBeNull();
    expect(loaded?.tick).toBe(sim.tick);
    expect(loaded?.organisms.length).toBe(sim.organisms.length);

    clearLocalSnapshot();
    expect(loadLocalSnapshot()).toBeNull();
  });
});
