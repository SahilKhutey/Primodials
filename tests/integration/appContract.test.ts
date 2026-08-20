import { describe, expect, it } from "vitest";
import { readFileSync } from "node:fs";
import { resolve } from "node:path";

const appSource = readFileSync(resolve(process.cwd(), "src/App.tsx"), "utf8");

describe("App integration contract", () => {
  it("integrates Phase 1 settings validation and persistence", () => {
    expect(appSource).toContain("usePersistentSettings");
    expect(appSource).toContain("validateSettings");
  });

  it("integrates local persistence", () => {
    expect(appSource).toContain("saveLocalSnapshot");
    expect(appSource).toContain("loadLocalSnapshot");
    expect(appSource).toContain("restoreSimulationSnapshot");
  });

  it("integrates sharing", () => {
    expect(appSource).toContain("readWorldFromUrl");
    expect(appSource).toContain("WorldShareDialog");
  });

  it("integrates runtime recovery and wallpaper visibility", () => {
    expect(appSource).toContain("useRuntimeRecovery");
    expect(appSource).toContain("installVisibilityController");
    expect(appSource).toContain("useAutosave");
  });

  it("integrates Phase 4 UX shell", () => {
    expect(appSource).toContain("Phase4UXShell");
    expect(appSource).toContain("SettingsSection");
    expect(appSource).toContain("SettingsRow");
  });

  it("keeps Wallpaper Engine bridge behind the wallpaper-only flag", () => {
    expect(appSource).toContain("WALLPAPER_ONLY");
    expect(appSource).toContain("installWallpaperEngineBridge");
  });
});
