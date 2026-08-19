/**
 * seededWorld — deterministic world creation for shareable seeds
 *
 * Given a seed and the same SimSettings, two users will see
 * identical initial populations. Useful for:
 *  - Bug reproduction (replay exact failing world)
 *  - Sharing interesting setups via seed strings
 *  - Tutorial / onboarding (curated seeds)
 *
 * License: MIT
 */

import type { SimSettings } from "@/sim/types";
import { Simulation } from "./simulation";

/**
 * Generate a cryptographically-strong 32-bit seed where available,
 * falling back to Math.random for environments without crypto.
 */
export function generateSeed(): number {
  if (typeof crypto !== "undefined" && "getRandomValues" in crypto) {
    const buf = new Uint32Array(1);
    crypto.getRandomValues(buf);
    return buf[0] >>> 0;
  }
  return (Date.now() ^ Math.floor(Math.random() * 0xffffffff)) >>> 0;
}

export function randomSeed(): number {
  return generateSeed();
}

/**
 * Convert a seed number to a short shareable string.
 * Example: 12345678 → "00bc614e"
 */
export function seedToString(seed: number): string {
  return (seed >>> 0).toString(16).padStart(8, "0");
}

/**
 * Parse a string back into a seed. Returns null if invalid.
 */
export function seedFromString(s: string): number | null {
  const n = parseInt(s, 16);
  if (Number.isNaN(n) || !Number.isFinite(n) || n < 0) return null;
  return n >>> 0;
}

/**
 * Create a Simulation with a known seed applied to its RNG.
 */
export function createSeededWorld(
  settings: SimSettings,
  options: { seed?: number; settings?: Partial<SimSettings> } = {}
): Simulation {
  const seed = options.seed ?? generateSeed();
  const effectiveSettings: SimSettings = {
    ...settings,
    ...(options.settings ?? {}),
  };
  return new Simulation(seed >>> 0, effectiveSettings);
}
