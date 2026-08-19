import { useSimulationStore } from '@/store/simulationStore';
import { cn } from '@/utils/cn';
import { Leaf, Fish, Droplet, Mountain, Sparkles, Flame } from 'lucide-react';

const ICONS: Record<string, typeof Leaf> = {
  'algae-bloom': Leaf,
  'deep-sea': Fish,
  'primordial-soup': Droplet,
  'tundra': Mountain,
  'coral-reef': Sparkles,
  'volcanic-vent': Flame,
};

export function BiomeSelector() {
  const { biomes, currentBiomeId, loadBiome } = useSimulationStore();

  return (
    <div className="flex-1 overflow-y-auto p-3">
      <h2 className="mb-3 px-2 font-mono text-xs uppercase tracking-wider text-text-muted">
        Biomes
      </h2>
      <ul className="space-y-1">
        {biomes.map((biome) => {
          const Icon = ICONS[biome.id] ?? Leaf;
          const isActive = biome.id === currentBiomeId;
          return (
            <li key={biome.id}>
              <button
                onClick={() => loadBiome(biome.id)}
                className={cn(
                  'group flex w-full items-center gap-3 rounded-md px-3 py-2 text-left transition-all',
                  isActive
                    ? 'bg-accent-magenta/15 text-text-primary ring-1 ring-accent-magenta'
                    : 'text-text-muted hover:bg-bg-surface hover:text-text-primary'
                )}
              >
                <Icon
                  className={cn(
                    'h-4 w-4 flex-shrink-0',
                    isActive ? 'text-accent-magenta' : ''
                  )}
                />
                <div className="flex-1 min-w-0">
                  <div className="truncate text-sm font-medium">
                    {biome.name}
                  </div>
                  <div className="truncate font-mono text-xs opacity-70">
                    {biome.species.length} species
                  </div>
                </div>
              </button>
            </li>
          );
        })}
      </ul>
    </div>
  );
}
