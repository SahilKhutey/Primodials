import { describe, expect, it } from "vitest";
import { readFileSync } from "node:fs";
import { resolve } from "node:path";

const pkg = JSON.parse(
  readFileSync(resolve(process.cwd(), "package.json"), "utf8"),
);

describe("package integration contract", () => {
  it("has the complete development verification chain", () => {
    for (const script of [
      "typecheck",
      "lint",
      "test",
      "build",
      "build:wallpaper",
      "package:wallpaper",
    ]) {
      expect(pkg.scripts?.[script]).toBeTruthy();
    }
  });

  it("has release packaging targets", () => {
    expect(pkg.scripts?.["package:itch"]).toBeTruthy();
    expect(pkg.scripts?.["package:all"]).toBeTruthy();
  });

  it("has Vitest installed", () => {
    expect(pkg.devDependencies?.vitest).toBeTruthy();
  });
});
