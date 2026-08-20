import { describe, expect, it } from "vitest";
import { readFileSync } from "node:fs";
import { resolve } from "node:path";

const source = readFileSync(
  resolve(process.cwd(), "src/components/SimCanvas.tsx"),
  "utf8",
);

describe("SimCanvas integration contract", () => {
  it("uses a single animation loop", () => {
    const occurrences = source.match(/requestAnimationFrame\(loop\)/g) ?? [];
    expect(occurrences.length).toBeGreaterThanOrEqual(2);
  });

  it("keeps the fixed simulation step", () => {
    expect(source).toContain("const STEP_MS = 1000 / 30");
  });

  it("caps elapsed time against a spiral of death", () => {
    expect(source).toContain("Math.min(now - last, 100)");
  });

  it("integrates performance and watchdog instrumentation", () => {
    expect(source).toContain("performanceRef.current.beginFrame");
    expect(source).toContain("watchdogRef.current.frame");
    expect(source).toContain("telemetryRef.current.watchdog");
  });

  it("resizes the canvas using device pixel ratio", () => {
    expect(source).toContain("window.devicePixelRatio");
    expect(source).toContain("canvas.width");
    expect(source).toContain("canvas.height");
  });
});
