import { create } from 'zustand';
import type { Biome } from '@/types/biome';
import type { SimulationParameters, SimulationStats } from '@/types/simulation';
import { biomes as allBiomes, getBiome } from '@/biomes';

interface SimulationStore {
  biomes: Biome[];
  currentBiomeId: string;
  currentBiome: Biome | null;
  seed: number;
  parameters: SimulationParameters;
  stats: SimulationStats;
  isRunning: boolean;
  isPaused: boolean;

  // Actions
  setIsRunning: (running: boolean) => void;
  loadBiome: (id: string) => void;
  setSeed: (seed: number) => void;
  setParameters: (params: Partial<SimulationParameters>) => void;
  setStats: (stats: SimulationStats) => void;
  togglePause: () => void;
  reset: () => void;
}

const DEFAULT_PARAMS: SimulationParameters = {
  preyReproductionRate: 1.0,
  predatorHuntSuccess: 1.0,
  carryingCapacity: 1000,
  speedMultiplier: 1.0,
  paused: false,
};

const DEFAULT_STATS: SimulationStats = {
  prey: 0,
  predators: 0,
  total: 0,
  fps: 60,
  ticks: 0,
  generations: 0,
};

export const useSimulationStore = create<SimulationStore>((set) => ({
  biomes: allBiomes,
  currentBiomeId: allBiomes[0]?.id ?? 'algae-bloom',
  currentBiome: allBiomes[0] ?? null,
  seed: Date.now(),
  parameters: DEFAULT_PARAMS,
  stats: DEFAULT_STATS,
  isRunning: false,
  isPaused: false,

  setIsRunning: (running) => set({ isRunning: running }),

  loadBiome: (id) => {
    const biome = getBiome(id);
    if (!biome) return;
    set({
      currentBiomeId: id,
      currentBiome: biome,
      seed: Date.now(),
      isRunning: true,
    });
  },

  setSeed: (seed) => set({ seed }),

  setParameters: (params) =>
    set((state) => ({
      parameters: { ...state.parameters, ...params },
    })),

  setStats: (stats) => set({ stats }),

  togglePause: () =>
    set((state) => ({
      isPaused: !state.isPaused,
      parameters: { ...state.parameters, paused: !state.parameters.paused },
    })),

  reset: () =>
    set({
      seed: Date.now(),
      stats: DEFAULT_STATS,
    }),
}));
