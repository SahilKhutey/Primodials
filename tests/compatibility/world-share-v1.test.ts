import { describe, expect, it } from "vitest";
import { createWorldShare, decodeWorldShare } from "@/lib/worldShare";
import { DEFAULT_SETTINGS } from "@/sim/types";

describe("Compatibility — V1 World Share Protocol", () => {
  it("encodes and decodes with canonical PP1 protocol format", () => {
    const payload = {
      seed: 424242,
      name: "Primordial Oasis",
      settings: {
        ...DEFAULT_SETTINGS,
        maxPopulation: 350,
        mutationRate: 0.12,
      },
    };

    const shareCode = createWorldShare(payload);
    expect(shareCode.startsWith("PP1.")).toBe(true);

    const decoded = decodeWorldShare(shareCode);
    expect(decoded).not.toBeNull();
    expect(decoded?.seed).toBe(424242);
    expect(decoded?.name).toBe("Primordial Oasis");
    expect(decoded?.settings.maxPopulation).toBe(350);
    expect(decoded?.settings.mutationRate).toBeCloseTo(0.12);
  });
});
