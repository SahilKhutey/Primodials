import { describe, expect, it } from "vitest";
import { AdaptivePerformanceController } from "@/lib/adaptivePerformance";

const slow = {
  fps: 20,
  frameMs: 50,
  simulationSteps: 30,
  population: 500,
};

const fast = {
  fps: 60,
  frameMs: 12,
  simulationSteps: 30,
  population: 100,
};

describe("AdaptivePerformanceController", () => {
  it("downshifts after sustained pressure", () => {
    const controller = new AdaptivePerformanceController("high", 45);

    for (let i = 0; i < 4; i += 1) controller.observe(slow);

    expect(controller.getState().quality).toBe("medium");
  });

  it("does not immediately oscillate upward", () => {
    const controller = new AdaptivePerformanceController("medium", 45);
    controller.observe(fast);
    expect(controller.getState().quality).toBe("medium");
  });

  it("can recover upward only after sustained stability", () => {
    const controller = new AdaptivePerformanceController("medium", 45);

    for (let i = 0; i < 8; i += 1) controller.observe(fast);

    expect(controller.getState().quality).toBe("high");
  });
});
