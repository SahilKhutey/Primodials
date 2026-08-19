import { useSimulationStore } from '@/store/simulationStore';
import { cn } from '@/utils/cn';

interface StatsOverlayProps {
  minimal?: boolean;
}

export function StatsOverlay({ minimal = false }: StatsOverlayProps) {
  const { stats, currentBiome } = useSimulationStore();

  if (minimal) {
    return (
      <div className="pointer-events-none absolute right-4 top-4 font-mono text-xs text-text-primary/40">
        {currentBiome?.name} · {stats.fps}fps · {stats.total} organisms
      </div>
    );
  }

  return (
    <div className="pointer-events-none absolute right-4 top-4 flex flex-col gap-2 font-mono text-xs">
      <Stat label="FPS" value={stats.fps.toString()} accent />
      <Stat label="Total" value={stats.total.toString()} />
      <Stat label="Prey" value={stats.prey.toString()} color="#4ECDC4" />
      <Stat label="Predators" value={stats.predators.toString()} color="#FF6B6B" />
      <Stat label="Ticks" value={stats.ticks.toLocaleString()} muted />
    </div>
  );
}

function Stat({
  label,
  value,
  color,
  accent,
  muted,
}: {
  label: string;
  value: string;
  color?: string;
  accent?: boolean;
  muted?: boolean;
}) {
  return (
    <div
      className={cn(
        'rounded-md border border-bg-surface/60 bg-bg-deep/70 px-3 py-1.5 backdrop-blur-md',
        muted && 'opacity-60'
      )}
    >
      <span className="text-text-muted">{label}: </span>
      <span
        className={cn(
          'font-bold',
          accent && 'text-accent-magenta'
        )}
        style={color ? { color } : undefined}
      >
        {value}
      </span>
    </div>
  );
}
