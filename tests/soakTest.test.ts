import { describe, expect, it } from "vitest";
import { DEFAULT_SETTINGS } from "../src/sim/types";
import { runSoakTest } from "../src/lib/soakTest";

describe("simulation soak", () => {
  it("runs a deterministic short soak without non-finite state", () => {
    const result = runSoakTest(
      123456,
      {
        ...DEFAULT_SETTINGS,
        initialPopulation: 25,
        maxPopulation: 100,
        initialFood: 50,
        maxFood: 100,
      },
      2,
    );

    expect(result.completed).toBe(true);
    expect(result.finiteState).toBe(true);
    expect(result.actualTicks).toBe(result.targetTicks);
    expect(result.maxPopulation).toBeLessThanOrEqual(DEFAULT_SETTINGS.maxPopulation * 2);
  });
});
