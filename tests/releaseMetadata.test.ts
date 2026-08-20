import { describe, expect, it } from "vitest";
import { PRODUCT_CHANNEL, PRODUCT_NAME, PRODUCT_VERSION } from "../src/lib/version";

describe("release metadata", () => {
  it("uses the canonical product name", () => {
    expect(PRODUCT_NAME).toBe("Polygonal Primordials");
  });

  it("uses a valid version", () => {
    expect(PRODUCT_VERSION).toMatch(/^\d+\.\d+\.\d+$/);
  });

  it("uses a supported release channel", () => {
    expect(["development", "release-candidate", "stable"]).toContain(PRODUCT_CHANNEL);
  });
});
