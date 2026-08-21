import { describe, expect, it } from "vitest";
import { existsSync, readFileSync } from "node:fs";
import { resolve } from "node:path";

describe("release tooling", () => {
  it("contains all release scripts", () => {
    const required = [
      ["Tools/validate-release.mjs", "tools/validate-release.mjs"],
      ["Tools/create-build-info.mjs", "tools/create-build-info.mjs"],
      ["Tools/generate-release-manifest.mjs", "tools/generate-release-manifest.mjs"],
    ];

    for (const candidates of required) {
      const exists = candidates.some((file) => existsSync(resolve(process.cwd(), file)));
      expect(exists, `Missing release tool. Tried: ${candidates.join(", ")}`).toBe(true);
    }
  });

  it("contains release checklist and notes", () => {
    expect(
      existsSync(resolve(process.cwd(), "release/RELEASE_CANDIDATE_CHECKLIST.md")),
    ).toBe(true);

    const notes = readFileSync(
      resolve(process.cwd(), "release/RELEASE_NOTES_1.0.0_RC1.md"),
      "utf8",
    );

    expect(notes).toContain("Polygonal Primordials 1.0.0 RC1");
  });
});
