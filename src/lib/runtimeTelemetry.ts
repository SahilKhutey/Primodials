import type { PerformanceSample } from "./performanceMonitor";

export type RuntimeTelemetry = {
  startedAt: string;
  uptimeSeconds: number;
  frames: number;
  simulationSteps: number;
  lastPopulation: number;
  lastSpecies: number;
  lastGeneration: number;
  crashesRecovered: number;
  watchdogEvents: number;
  lastPerformance: PerformanceSample | null;
};

const MAX_SAMPLES = 120;

export class RuntimeTelemetryCollector {
  private readonly startedAt = Date.now();
  private frames = 0;
  private simulationSteps = 0;
  private crashesRecovered = 0;
  private watchdogEvents = 0;
  private lastPopulation = 0;
  private lastSpecies = 0;
  private lastGeneration = 0;
  private samples: PerformanceSample[] = [];

  frame(sample: PerformanceSample | null) {
    this.frames += 1;
    if (!sample) return;
    this.lastPerformance(sample);
  }

  lastPerformance(sample: PerformanceSample) {
    this.samples.push(sample);
    if (this.samples.length > MAX_SAMPLES) this.samples.shift();
    this.lastPopulation = Math.round(sample.population);
  }

  simulation(steps: number, species: number, generation: number) {
    this.simulationSteps += steps;
    this.lastSpecies = species;
    this.lastGeneration = generation;
  }

  crashRecovered() {
    this.crashesRecovered += 1;
  }

  watchdog() {
    this.watchdogEvents += 1;
  }

  snapshot(): RuntimeTelemetry {
    return {
      startedAt: new Date(this.startedAt).toISOString(),
      uptimeSeconds: (Date.now() - this.startedAt) / 1000,
      frames: this.frames,
      simulationSteps: this.simulationSteps,
      lastPopulation: this.lastPopulation,
      lastSpecies: this.lastSpecies,
      lastGeneration: this.lastGeneration,
      crashesRecovered: this.crashesRecovered,
      watchdogEvents: this.watchdogEvents,
      lastPerformance: this.samples.length > 0 ? this.samples[this.samples.length - 1] : null,
    };
  }

  exportJson(): string {
    return JSON.stringify(
      {
        ...this.snapshot(),
        performanceSamples: this.samples,
      },
      null,
      2,
    );
  }
}
