import { describe, expect, it } from "vitest";
import { existsSync, readFileSync } from "node:fs";
import { resolve } from "node:path";

describe("publishing assets", () => {
  it("contains the Steam store template", () => {
    const path = resolve(process.cwd(), "release/steam/STEAM_STORE_TEMPLATE.md");
    expect(existsSync(path)).toBe(true);
    const content = readFileSync(path, "utf8");
    expect(content).toContain("Polygonal Primordials");
    expect(content).toContain("Only publish");
  });

  it("contains the Wallpaper Engine checklist", () => {
    expect(
      existsSync(resolve(
        process.cwd(),
        "release/wallpaper-engine/WORKSHOP_RELEASE_CHECKLIST.md",
      )),
    ).toBe(true);
  });
});
