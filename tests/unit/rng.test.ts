import { describe, expect, it } from "vitest";
import { Rng } from "@/sim/rng";

describe("Rng", () => {
  it("is deterministic for the same seed", () => {
    const a = new Rng(12345);
    const b = new Rng(12345);

    expect(Array.from({ length: 20 }, () => a.next()))
      .toEqual(Array.from({ length: 20 }, () => b.next()));
  });

  it("produces values inside [0, 1)", () => {
    const rng = new Rng(99);
    for (let i = 0; i < 500; i += 1) {
      const value = rng.next();
      expect(value).toBeGreaterThanOrEqual(0);
      expect(value).toBeLessThan(1);
    }
  });

  it("supports bounded range and integer generation", () => {
    const rng = new Rng(42);
    for (let i = 0; i < 100; i += 1) {
      expect(rng.range(-5, 7)).toBeGreaterThanOrEqual(-5);
      expect(rng.range(-5, 7)).toBeLessThan(7);

      const n = rng.int(2, 5);
      expect(n).toBeGreaterThanOrEqual(2);
      expect(n).toBeLessThanOrEqual(5);
    }
  });
});
