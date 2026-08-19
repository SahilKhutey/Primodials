/**
 * localSnapshot — offline save / load for the running simulation
 *
 * The existing App.tsx has a Supabase cloud snapshot path.
 * This adds an OFFLINE-FIRST path that works without any account
 * or internet connection.
 *
 * License: MIT
 */

import { safeGetJSON, safeSetJSON, safeRemove } from "@/lib/safeStorage";
import type { SimSettings, SimStats, Species, Colony, Structure, Biome, Remains, KnowledgeNode, Food, Organism } from "./types";
import type { Simulation } from "./simulation";

const SNAPSHOT_KEY = "primordials:snapshot:v1";
const SNAPSHOT_INDEX_KEY = "primordials:snapshots:index:v1";

export type LocalSimulationSnapshot = {
  version: 1;
  savedAt: string;
  createdAt: number;
  tick: number;
  organisms: Array<Omit<Organism, "brain"> & {
    brain: null | {
      nInputs: number;
      nHidden: number;
      nOutputs: number;
      weights: number[];
    };
  }>;
  food: Food[];
  species: Species[];
  colonies: Colony[];
  structures: Structure[];
  biomes: Biome[];
  remains: Remains[];
  knowledgeNodes: KnowledgeNode[];
  stats: SimStats;
  settings: SimSettings;
  state?: unknown;
};

export type SimulationSnapshot = LocalSimulationSnapshot;

function encodeBrain(organism: Organism) {
  if (!organism.brain) return null;
  return {
    nInputs: organism.brain.nInputs,
    nHidden: organism.brain.nHidden,
    nOutputs: organism.brain.nOutputs,
    weights: Array.from(organism.brain.weights),
  };
}

export function captureLocalSnapshot(sim: Simulation): LocalSimulationSnapshot {
  const now = Date.now();
  return {
    version: 1,
    savedAt: new Date(now).toISOString(),
    createdAt: now,
    tick: sim.tick,
    organisms: sim.organisms.map((o) => ({
      ...o,
      brain: encodeBrain(o),
      lastInputs: null,
      lastOutputs: null,
    })),
    food: sim.food,
    species: sim.species,
    colonies: sim.colonies,
    structures: sim.structures,
    biomes: sim.biomes,
    remains: sim.remains,
    knowledgeNodes: sim.knowledgeNodes,
    stats: sim.stats,
    settings: sim.settings,
  };
}

/**
 * Save the current simulation state to localStorage.
 * Returns true on success, false if storage failed.
 */
export function saveLocalSnapshot(sim: unknown): boolean {
  try {
    if (!sim) return false;
    const simTyped = sim as Simulation;
    const snapshot = captureLocalSnapshot(simTyped);

    if (!safeSetJSON(SNAPSHOT_KEY, snapshot)) return false;

    // Also update index
    const index = safeGetJSON<string[]>(SNAPSHOT_INDEX_KEY) ?? [];
    index.push(`snapshot-${snapshot.createdAt}`);
    safeSetJSON(SNAPSHOT_INDEX_KEY, index.slice(-20));

    return true;
  } catch {
    return false;
  }
}

/**
 * Load the most recent local snapshot.
 * Returns null if no snapshot exists or it can't be parsed.
 */
export function loadLocalSnapshot(): LocalSimulationSnapshot | null {
  return safeGetJSON<LocalSimulationSnapshot>(SNAPSHOT_KEY);
}

/**
 * List all known snapshot identifiers.
 */
export function listLocalSnapshots(): string[] {
  return safeGetJSON<string[]>(SNAPSHOT_INDEX_KEY) ?? [];
}

/**
 * Delete the local snapshot.
 */
export function clearLocalSnapshot(): void {
  safeRemove(SNAPSHOT_KEY);
}
