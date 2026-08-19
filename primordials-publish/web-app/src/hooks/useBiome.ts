import { useSimulationStore } from '@/store/simulationStore';

export function useBiome() {
  const { currentBiome, biomes, loadBiome } = useSimulationStore();
  return {
    biome: currentBiome,
    biomes,
    loadBiome,
  };
}
