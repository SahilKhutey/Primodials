import { describe, expect, it } from "vitest";

describe("fault injection safety", () => {
  it("module exposes the supported API", async () => {
    const module = await import("../../faults/faultInjection");
    expect(typeof module.activeFault).toBe("function");
    expect(typeof module.shouldFail).toBe("function");
    expect(typeof module.assertNoFault).toBe("function");
  });
});
