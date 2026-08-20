import { describe, expect, it } from "vitest";
import { validateSettings } from "@/lib/settingsValidation";
import { DEFAULT_SETTINGS } from "@/sim/types";

describe("Compatibility — Schema Migration Helper", () => {
  it("safely migrates legacy settings dictionary to current schema", () => {
    const legacySettings = {
      worldWidth: 1000,
      worldHeight: 700,
      initialPopulation: 60,
      mutationRate: 0.08,
      // legacy field names or deprecated properties
      oldMutationEngine: true,
      deprecatedField: "something",
    };

    const migrated = validateSettings(legacySettings, DEFAULT_SETTINGS);

    // Current standard properties are preserved
    expect(migrated.worldWidth).toBe(1000);
    expect(migrated.worldHeight).toBe(700);
    expect(migrated.initialPopulation).toBe(60);
    expect(migrated.mutationRate).toBe(0.08);

    // Deprecated fields are dropped or omitted from SimSettings
    expect((migrated as unknown as Record<string, unknown>).oldMutationEngine).toBeUndefined();
    expect((migrated as unknown as Record<string, unknown>).deprecatedField).toBeUndefined();

    // Required schema defaults are backfilled
    expect(migrated.boundaryMode).toBe("wrap");
    expect(migrated.foodGrowthRate).toBe(DEFAULT_SETTINGS.foodGrowthRate);
  });
});
