import { describe, expect, it } from "vitest";
import { createWorldShare, decodeWorldShare } from "../src/lib/worldShare";

describe("world sharing", () => {
  it("round trips seed and settings", () => {
    const code = createWorldShare(
      123456,
      { worldWidth: 2800, initialPopulation: 75, boundaryMode: "wrap" },
      "Test World",
    );

    const decoded = decodeWorldShare(code);

    expect(decoded).not.toBeNull();
    expect(decoded?.seed).toBe(123456);
    expect(decoded?.settings.initialPopulation).toBe(75);
    expect(decoded?.name).toBe("Test World");
  });

  it("rejects invalid codes", () => {
    expect(decodeWorldShare("bad-code")).toBeNull();
    expect(decodeWorldShare("PP2.abc")).toBeNull();
  });
});
