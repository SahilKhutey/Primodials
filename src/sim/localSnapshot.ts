import type { SimSettings, SimStats, Species, Colony, Structure, Biome, Remains, KnowledgeNode, Food, Organism } from "./types";
import type { Simulation } from "./simulation";

const STORAGE_KEY = "polygonal-primordials.snapshot.v1";

export type LocalSimulationSnapshot = {
  version: 1;
  savedAt: string;
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
};

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
  return {
    version: 1,
    savedAt: new Date().toISOString(),
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

export function saveLocalSnapshot(sim: Simulation): boolean {
  if (typeof window === "undefined") return false;
  try {
    window.localStorage.setItem(STORAGE_KEY, JSON.stringify(captureLocalSnapshot(sim)));
    return true;
  } catch {
    // Storage failures must never crash the simulation.
    return false;
  }
}

export function loadLocalSnapshot(): LocalSimulationSnapshot | null {
  if (typeof window === "undefined") return null;
  try {
    const raw = window.localStorage.getItem(STORAGE_KEY);
    if (!raw) return null;
    const parsed = JSON.parse(raw) as LocalSimulationSnapshot;
    if (parsed?.version !== 1) return null;
    return parsed;
  } catch {
    return null;
  }
}

export function clearLocalSnapshot(): boolean {
  if (typeof window === "undefined") return false;
  try {
    window.localStorage.removeItem(STORAGE_KEY);
    return true;
  } catch {
    // Intentionally ignored.
    return false;
  }
}
