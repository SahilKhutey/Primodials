import { describe, expect, it } from "vitest";
import { PerformanceMonitor } from "@/lib/performanceMonitor";

describe("PerformanceMonitor", () => {
  it("does not emit a sample before the minimum interval", () => {
    const monitor = new PerformanceMonitor();
    monitor.beginFrame(0);
    monitor.record(1, 10, 16);
    expect(monitor.snapshot()).toBeNull();
  });

  it("eventually creates samples", () => {
    const monitor = new PerformanceMonitor();

    monitor.beginFrame(0);
    for (let i = 1; i <= 40; i += 1) {
      monitor.beginFrame(i * 20);
      monitor.record(1, 10, 20);
    }

    expect(monitor.snapshot()).not.toBeNull();
    expect(monitor.average()).not.toBeNull();
  });
});
