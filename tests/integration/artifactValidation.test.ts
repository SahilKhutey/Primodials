import { describe, expect, it } from "vitest";
import { existsSync } from "node:fs";
import { resolve } from "node:path";

describe("external QA tooling", () => {
  it("contains all required QA tools", () => {
    for (const path of [
      "playwright.config.ts",
      "tools/run-browser-benchmark.mjs",
      "scripts/hardware-report.ps1",
      "scripts/windows-release-artifact-check.ps1",
      "faults/faultInjection.ts",
    ]) {
      expect(existsSync(resolve(process.cwd(), path))).toBe(true);
    }
  });
});
