import { afterEach, describe, expect, it } from "vitest";
import { DEFAULT_SETTINGS } from "@/sim/types";
import {
  createWorldShare,
  decodeWorldShare,
  encodeWorldShare,
  readWorldFromUrl,
  shareToUrl,
} from "@/lib/worldShare";
import { installMemoryBrowser } from "../fixtures/testStorage";

afterEach(() => {
  Reflect.deleteProperty(globalThis, "window");
  Reflect.deleteProperty(globalThis, "localStorage");
});

describe("world sharing", () => {
  it("round-trips version, seed, name and settings", () => {
    const code = createWorldShare(
      123456,
      {
        worldWidth: 3200,
        initialPopulation: 120,
        mutationRate: 0.2,
        boundaryMode: "wrap",
      },
      "Test World",
    );

    const decoded = decodeWorldShare(code);

    expect(decoded).not.toBeNull();
    expect(decoded?.version).toBe(1);
    expect(decoded?.seed).toBe(123456);
    expect(decoded?.name).toBe("Test World");
    expect(decoded?.settings.initialPopulation).toBe(120);
  });

  it("rejects malformed and incompatible codes", () => {
    expect(decodeWorldShare("invalid")).toBeNull();
    expect(decodeWorldShare("PP2.abc")).toBeNull();
    expect(decodeWorldShare("PP1.")).toBeNull();

    const incompatible = encodeWorldShare({
      version: 1,
      seed: 1,
      settings: {},
    });
    expect(decodeWorldShare(incompatible)).not.toBeNull();
  });

  it("creates a URL containing the world parameter", () => {
    installMemoryBrowser("https://primordials.test/play");
    const code = createWorldShare(7, DEFAULT_SETTINGS);
    const url = shareToUrl(code);

    expect(url).toContain("world=PP1");
  });

  it("reads a shared world from the URL", () => {
    const code = createWorldShare(987, { maxPopulation: 300 });
    installMemoryBrowser(`https://primordials.test/?world=${encodeURIComponent(code)}`);

    const shared = readWorldFromUrl();

    expect(shared?.seed).toBe(987);
    expect(shared?.settings.maxPopulation).toBe(300);
  });
});
