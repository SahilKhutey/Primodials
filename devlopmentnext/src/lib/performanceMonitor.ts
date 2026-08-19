export type PerformanceSample = {
  fps: number;
  frameMs: number;
  simulationSteps: number;
  population: number;
};

export class PerformanceMonitor {
  private last = performance.now();
  private frameCount = 0;
  private elapsed = 0;
  private samples: PerformanceSample[] = [];

  beginFrame(now = performance.now()): number {
    const dt = Math.max(0, now - this.last);
    this.last = now;
    this.frameCount += 1;
    this.elapsed += dt;
    return dt;
  }

  record(simulationSteps: number, population: number, frameMs: number) {
    if (this.elapsed < 500) return;
    const fps = (this.frameCount / this.elapsed) * 1000;
    this.samples.push({ fps, frameMs, simulationSteps, population });
    if (this.samples.length > 60) this.samples.shift();
    this.frameCount = 0;
    this.elapsed = 0;
  }

  snapshot(): PerformanceSample | null {
    return this.samples.length ? this.samples[this.samples.length - 1] : null;
  }

  average(windowSize = 30): PerformanceSample | null {
    if (!this.samples.length) return null;
    const data = this.samples.slice(-windowSize);
    const avg = (key: keyof PerformanceSample) =>
      data.reduce((sum, item) => sum + Number(item[key]), 0) / data.length;
    return {
      fps: avg("fps"),
      frameMs: avg("frameMs"),
      simulationSteps: avg("simulationSteps"),
      population: avg("population"),
    };
  }
}
