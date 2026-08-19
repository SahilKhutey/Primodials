import { useSimulationStore } from '@/store/simulationStore';
import { Slider } from './ui/Slider';
import { Pause, Play, RotateCcw } from 'lucide-react';

export function ParameterPanel() {
  const { parameters, setParameters, togglePause, reset, currentBiome } = useSimulationStore();

  if (!currentBiome) return null;

  return (
    <div className="flex flex-col gap-4 overflow-y-auto p-4">
      <section>
        <h2 className="mb-3 font-mono text-xs uppercase tracking-wider text-text-muted">Controls</h2>
        <div className="flex gap-2">
          <button
            onClick={togglePause}
            className="inline-flex flex-1 items-center justify-center gap-2 rounded-md border border-bg-surface bg-bg-surface/40 px-4 py-2 text-sm font-medium text-text-primary transition-all hover:border-accent-magenta"
          >
            {parameters.paused ? (
              <>
                <Play className="h-3.5 w-3.5" /> Resume
              </>
            ) : (
              <>
                <Pause className="h-3.5 w-3.5" /> Pause
              </>
            )}
          </button>
          <button
            onClick={reset}
            className="inline-flex items-center justify-center rounded-md p-2 text-text-muted transition-all hover:bg-bg-surface hover:text-text-primary"
          >
            <RotateCcw className="h-3.5 w-3.5" />
          </button>
        </div>
      </section>

      <section>
        <h2 className="mb-3 font-mono text-xs uppercase tracking-wider text-text-muted">Parameters</h2>
        <div className="space-y-4">
          <Slider
            label="Carrying Capacity"
            value={parameters.carryingCapacity}
            min={100}
            max={3000}
            step={50}
            onChange={(v) => setParameters({ carryingCapacity: v })}
          />
          <Slider
            label="Prey Reproduction"
            value={parameters.preyReproductionRate}
            min={0.1}
            max={3}
            step={0.05}
            onChange={(v) => setParameters({ preyReproductionRate: v })}
          />
          <Slider
            label="Predator Success"
            value={parameters.predatorHuntSuccess}
            min={0.1}
            max={2}
            step={0.05}
            onChange={(v) => setParameters({ predatorHuntSuccess: v })}
          />
          <Slider
            label="Speed Multiplier"
            value={parameters.speedMultiplier}
            min={0.1}
            max={3}
            step={0.1}
            onChange={(v) => setParameters({ speedMultiplier: v })}
          />
        </div>
      </section>

      <section>
        <h2 className="mb-3 font-mono text-xs uppercase tracking-wider text-text-muted">Species</h2>
        <div className="space-y-2">
          {currentBiome.species.map((s) => (
            <div
              key={s.id}
              className="flex items-center gap-2 rounded-md bg-bg-surface/30 p-2 text-sm"
            >
              <div
                className="h-3 w-3 rounded-full"
                style={{ backgroundColor: s.color }}
              />
              <span className="flex-1 text-xs font-medium">{s.name}</span>
              <span className="font-mono text-xs text-text-muted">
                ×{s.initial}
              </span>
            </div>
          ))}
        </div>
      </section>
    </div>
  );
}
