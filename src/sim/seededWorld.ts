import { Simulation } from "./simulation";
import type { SimSettings } from "./types";

export type SeededWorldOptions = {
  seed: number;
  settings?: Partial<SimSettings>;
};

export function createSeededWorld(
  defaults: SimSettings,
  options: SeededWorldOptions,
): Simulation {
  const settings: SimSettings = { ...defaults, ...options.settings };
  return new Simulation(options.seed >>> 0, settings);
}

export function randomSeed(): number {
  if (typeof crypto !== "undefined" && "getRandomValues" in crypto) {
    const buffer = new Uint32Array(1);
    crypto.getRandomValues(buffer);
    return buffer[0] >>> 0;
  }
  return (Date.now() ^ Math.floor(Math.random() * 0xffffffff)) >>> 0;
}
