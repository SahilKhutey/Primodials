/**
 * performanceMonitor — lightweight FPS / frame / sim-step telemetry
 *
 * - Rolling window (last N frames)
 * - Tracks frame ms, sim steps per frame, population
 * - Designed to be cheap (no allocations in hot path)
 *
 * License: MIT
 */

export interface PerformanceSnapshot {
  fps: number;
  frameMs: number;
  p99FrameMs: number;
  simStepsPerSec: number;
  population: number;
  samples: number;
}

export type PerformanceSample = PerformanceSnapshot;

interface RollingBuffer {
  data: number[];
  index: number;
  count: number;
  capacity: number;
}

function createRolling(capacity: number): RollingBuffer {
  return { data: new Array(capacity).fill(0), index: 0, count: 0, capacity };
}

function pushRolling(buf: RollingBuffer, value: number): void {
  buf.data[buf.index] = value;
  buf.index = (buf.index + 1) % buf.capacity;
  if (buf.count < buf.capacity) buf.count++;
}

function statsRolling(buf: RollingBuffer): { avg: number; p99: number } {
  if (buf.count === 0) return { avg: 0, p99: 0 };
  const sorted = buf.data.slice(0, buf.count).sort((a, b) => a - b);
  const avg = sorted.reduce((s, v) => s + v, 0) / buf.count;
  const p99Index = Math.min(sorted.length - 1, Math.floor(sorted.length * 0.99));
  return { avg, p99: sorted[p99Index] ?? avg };
}

export class PerformanceMonitor {
  private frameMsBuf = createRolling(60);
  private stepsBuf = createRolling(60);
  private lastSnapshotMs = 0;
  private snapshotIntervalMs = 500;

  // Frame tracking
  private frameStartMs = 0;

  // Per-frame accumulators
  private stepsThisFrame = 0;
  private population = 0;

  beginFrame(now: number = performance.now()): number {
    const dt = this.frameStartMs === 0 ? 16.67 : now - this.frameStartMs;
    this.frameStartMs = now;
    this.stepsThisFrame = 0;
    return dt;
  }

  record(steps: number, population: number, frameMs: number): void {
    this.stepsThisFrame = steps;
    this.population = population;
    pushRolling(this.frameMsBuf, frameMs);
    pushRolling(this.stepsBuf, steps);

    const now = performance.now();
    if (now - this.lastSnapshotMs >= this.snapshotIntervalMs) {
      this.lastSnapshotMs = now;
    }
  }

  getSnapshot(): PerformanceSnapshot {
    const frames = statsRolling(this.frameMsBuf);
    const steps = statsRolling(this.stepsBuf);
    const avgFrame = frames.avg;
    const fps = avgFrame > 0 ? Math.round(1000 / avgFrame) : 0;
    const simStepsPerSec = Math.round(steps.avg * fps);
    return {
      fps,
      frameMs: Math.round(avgFrame * 10) / 10,
      p99FrameMs: Math.round(frames.p99 * 10) / 10,
      simStepsPerSec,
      population: this.population,
      samples: this.frameMsBuf.count,
    };
  }

  snapshot(): PerformanceSnapshot {
    return this.getSnapshot();
  }

  reset(): void {
    this.frameMsBuf = createRolling(60);
    this.stepsBuf = createRolling(60);
    this.frameStartMs = 0;
    this.lastSnapshotMs = 0;
    this.stepsThisFrame = 0;
    this.population = 0;
  }
}
