import { describe, expect, it } from "vitest";
import { existsSync, readFileSync } from "node:fs";
import { resolve } from "node:path";

describe("release tooling", () => {
  it("contains all release scripts", () => {
    for (const file of [
      "tools/validate-release.mjs",
      "tools/create-build-info.mjs",
      "tools/generate-release-manifest.mjs",
    ]) {
      expect(existsSync(resolve(process.cwd(), file))).toBe(true);
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
