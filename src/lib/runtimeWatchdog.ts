export type WatchdogState = {
  lastFrameAt: number;
  lastSimulationAt: number;
  consecutiveSlowFrames: number;
  consecutiveSimulationStalls: number;
};

export type WatchdogEvent =
  | { type: "slow-frame"; frameMs: number; thresholdMs: number }
  | { type: "simulation-stall"; elapsedMs: number; thresholdMs: number }
  | { type: "recovered" };

export class RuntimeWatchdog {
  private state: WatchdogState = {
    lastFrameAt: typeof performance !== "undefined" ? performance.now() : 0,
    lastSimulationAt: typeof performance !== "undefined" ? performance.now() : 0,
    consecutiveSlowFrames: 0,
    consecutiveSimulationStalls: 0,
  };

  private unhealthy = false;

  constructor(
    private readonly slowFrameThresholdMs = 100,
    private readonly stallThresholdMs = 2000,
    private readonly violationLimit = 5,
  ) {}

  frame(frameMs: number, simulationAdvanced: boolean): WatchdogEvent | null {
    const now = typeof performance !== "undefined" ? performance.now() : Date.now();
    this.state.lastFrameAt = now;

    if (frameMs >= this.slowFrameThresholdMs) {
      this.state.consecutiveSlowFrames += 1;
    } else {
      this.state.consecutiveSlowFrames = Math.max(
        0,
        this.state.consecutiveSlowFrames - 1,
      );
    }

    if (simulationAdvanced) {
      this.state.lastSimulationAt = now;
      this.state.consecutiveSimulationStalls = 0;
    } else if (now - this.state.lastSimulationAt >= this.stallThresholdMs) {
      this.state.consecutiveSimulationStalls += 1;
    }

    const unhealthyNow =
      this.state.consecutiveSlowFrames >= this.violationLimit ||
      this.state.consecutiveSimulationStalls >= this.violationLimit;

    if (unhealthyNow && !this.unhealthy) {
      this.unhealthy = true;

      if (this.state.consecutiveSimulationStalls >= this.violationLimit) {
        return {
          type: "simulation-stall",
          elapsedMs: now - this.state.lastSimulationAt,
          thresholdMs: this.stallThresholdMs,
        };
      }

      return {
        type: "slow-frame",
        frameMs,
        thresholdMs: this.slowFrameThresholdMs,
      };
    }

    if (!unhealthyNow && this.unhealthy) {
      this.unhealthy = false;
      return { type: "recovered" };
    }

    return null;
  }

  isUnhealthy(): boolean {
    return this.unhealthy;
  }

  snapshot() {
    return { ...this.state, unhealthy: this.unhealthy };
  }
}
