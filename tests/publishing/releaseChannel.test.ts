import { describe, expect, it } from "vitest";

describe("release channel", () => {
  it("exports a recognized channel", async () => {
    const module = await import("@/lib/releaseChannel");
    expect(["development", "qa", "beta", "stable"]).toContain(module.RELEASE_CHANNEL);
  });

  it("recognizes valid values", async () => {
    const { isReleaseChannel } = await import("@/lib/releaseChannel");
    expect(isReleaseChannel("development")).toBe(true);
    expect(isReleaseChannel("qa")).toBe(true);
    expect(isReleaseChannel("beta")).toBe(true);
    expect(isReleaseChannel("stable")).toBe(true);
    expect(isReleaseChannel("production")).toBe(false);
  });
});
