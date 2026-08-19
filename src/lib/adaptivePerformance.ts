import type { QualityPreset } from "@/hooks/useWallpaperSettings";
import type { PerformanceSample } from "./performanceMonitor";

export type AdaptivePerformanceState = {
  quality: QualityPreset;
  targetFps: number;
  pressure: number;
};

const order: QualityPreset[] = ["low", "medium", "high"];

function shiftQuality(current: QualityPreset, direction: -1 | 1): QualityPreset {
  const index = order.indexOf(current);
  const next = Math.max(0, Math.min(order.length - 1, index + direction));
  return order[next];
}

export class AdaptivePerformanceController {
  private state: AdaptivePerformanceState;
  private stableWindows = 0;
  private pressureWindows = 0;

  constructor(
    initial: QualityPreset = "medium",
    targetFps = 45,
  ) {
    this.state = { quality: initial, targetFps, pressure: 0 };
  }

  observe(sample: Partial<PerformanceSample> & { fps: number; frameMs: number }): QualityPreset | null {
    if (!Number.isFinite(sample.fps)) return null;

    const fpsRatio = sample.fps / this.state.targetFps;

    if (fpsRatio < 0.75 || sample.frameMs > 35) {
      this.pressureWindows += 1;
      this.stableWindows = 0;
    } else if (fpsRatio >= 1.15 && sample.frameMs < 20) {
      this.stableWindows += 1;
      this.pressureWindows = 0;
    } else {
      this.stableWindows = Math.max(0, this.stableWindows - 1);
      this.pressureWindows = Math.max(0, this.pressureWindows - 1);
    }

    if (this.pressureWindows >= 4) {
      this.pressureWindows = 0;
      this.state.pressure = Math.min(1, this.state.pressure + 0.2);
      const next = shiftQuality(this.state.quality, -1);
      if (next !== this.state.quality) {
        this.state.quality = next;
        return next;
      }
    }

    if (this.stableWindows >= 8 && this.state.pressure > 0) {
      this.stableWindows = 0;
      this.state.pressure = Math.max(0, this.state.pressure - 0.2);
      const next = shiftQuality(this.state.quality, 1);
      if (next !== this.state.quality) {
        this.state.quality = next;
        return next;
      }
    }

    return null;
  }

  getState(): AdaptivePerformanceState {
    return { ...this.state };
  }

  setQuality(quality: QualityPreset) {
    this.state.quality = quality;
    this.state.pressure = 0;
    this.stableWindows = 0;
    this.pressureWindows = 0;
  }
}
