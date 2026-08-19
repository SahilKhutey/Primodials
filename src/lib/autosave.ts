import type { Simulation } from "@/sim/simulation";
import { saveLocalSnapshot } from "@/sim/localSnapshot";
import { getSafeStorage } from "./safeStorage";

const META_KEY = "polygonal-primordials.autosave.meta.v1";

export type AutosaveMeta = {
  savedAt: string;
  tick: number;
  population: number;
  species: number;
};

export function writeAutosave(sim: Simulation): void {
  saveLocalSnapshot(sim);
  const storage = getSafeStorage();
  if (!storage) return;

  const meta: AutosaveMeta = {
    savedAt: new Date().toISOString(),
    tick: sim.tick,
    population: sim.population,
    species: sim.species.length,
  };

  try {
    storage.setItem(META_KEY, JSON.stringify(meta));
  } catch {
    // Non-fatal in privacy/quota restricted environments.
  }
}

export function readAutosaveMeta(): AutosaveMeta | null {
  const storage = getSafeStorage();
  if (!storage) return null;
  try {
    const raw = storage.getItem(META_KEY);
    return raw ? (JSON.parse(raw) as AutosaveMeta) : null;
  } catch {
    return null;
  }
}
