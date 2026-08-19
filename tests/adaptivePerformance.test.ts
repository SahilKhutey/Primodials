import { describe, expect, it } from "vitest";
import { AdaptivePerformanceController } from "../src/lib/adaptivePerformance";

describe("adaptive performance", () => {
  it("downshifts after sustained pressure", () => {
    const c = new AdaptivePerformanceController("high", 45);

    for (let i = 0; i < 4; i++) {
      c.observe({
        fps: 20,
        frameMs: 50,
        simStepsPerSec: 30,
        population: 300,
        samples: 60,
      });
    }

    expect(c.getState().quality).toBe("medium");
  });

  it("does not oscillate immediately", () => {
    const c = new AdaptivePerformanceController("medium", 45);
    c.observe({
      fps: 60,
      frameMs: 12,
      simStepsPerSec: 30,
      population: 100,
      samples: 60,
    });

    expect(c.getState().quality).toBe("medium");
  });
});
