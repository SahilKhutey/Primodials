import { describe, expect, it } from "vitest";
import { existsSync } from "node:fs";
import { resolve } from "node:path";

function findExisting(candidates: string[]): string | null {
  for (const candidate of candidates) {
    if (existsSync(resolve(process.cwd(), candidate))) return candidate;
  }
  return null;
}

describe("external QA tooling", () => {
  it("contains all required QA tools using repository-native paths", () => {
    const required = [
      ["playwright.config.ts"],
      ["Tools/run-browser-benchmark.mjs", "tools/run-browser-benchmark.mjs"],
      ["Scripts/hardware-report.ps1", "scripts/hardware-report.ps1"],
      [
        "Scripts/windows-release-artifact-check.ps1",
        "scripts/windows-release-artifact-check.ps1",
      ],
      ["faults/faultInjection.ts"],
    ];

    for (const candidates of required) {
      const resolved = findExisting(candidates);
      expect(
        resolved,
        `Missing required QA artifact. Tried: ${candidates.join(", ")}`,
      ).not.toBeNull();
    }
  });
});
