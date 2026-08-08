import { useEffect, useRef, useState } from 'react';
import { Activity, Dna, Layers, Brain, Globe } from 'lucide-react';
import type { Simulation } from '@/sim/simulation';

type Props = {
  sim: Simulation;
  visible: boolean;
};

// Minimal ambient HUD for wallpaper mode — shows just the essential
// stats in a subtle, elegant overlay that fades when not interacted with.
export function AmbientHUD({ sim, visible }: Props) {
  const [opacity, setOpacity] = useState(0.85);
  const lastInteraction = useRef(Date.now());

  useEffect(() => {
    const onMove = () => {
      lastInteraction.current = Date.now();
      setOpacity(0.95);
    };
    window.addEventListener('mousemove', onMove);
    return () => window.removeEventListener('mousemove', onMove);
  }, []);

  useEffect(() => {
    const id = setInterval(() => {
      if (Date.now() - lastInteraction.current > 4000) {
        setOpacity(0.2);
      } else {
        setOpacity(0.95);
      }
    }, 500);
    return () => clearInterval(id);
  }, []);

  if (!visible) return null;

  const stats = sim.stats;
  const tier = stats.avgIntelligence;

  return (
    <div
      className="pointer-events-none fixed left-6 top-6 z-30 flex flex-col gap-2.5 transition-opacity duration-1000"
      style={{ opacity }}
    >
      {/* Title */}
      <div className="flex items-center gap-3">
        <div className="relative">
          <div className="absolute -inset-1 rounded-xl bg-gradient-to-br from-cyan-500/30 to-violet-500/20 blur-sm" />
          <div className="relative rounded-xl bg-neutral-950/80 p-2 ring-1 ring-white/15 backdrop-blur-md shadow-lg shadow-black/50">
            <Dna className="text-cyan-300 animate-pulse" size={20} />
          </div>
        </div>
        <div>
          <div className="text-sm font-bold tracking-tight text-neutral-100 drop-shadow-md">Polygonal Primordials</div>
          <div className="text-[10px] font-medium tracking-wide text-neutral-400">Ecosystem Wallpaper</div>
        </div>
      </div>

      {/* Stats strip */}
      <div className="flex flex-wrap gap-1.5">
        <StatChip icon={<Activity size={11} />} label="Pop" value={sim.population} color="text-emerald-300" />
        <StatChip icon={<Layers size={11} />} label="Species" value={sim.species.length} color="text-cyan-300" />
        <StatChip icon={<Brain size={11} />} label="Intel" value={tier.toFixed(2)} color="text-violet-300" />
        <StatChip icon={<Dna size={11} />} label="Gen" value={stats.maxGeneration} color="text-amber-300" />
        <StatChip icon={<Globe size={11} />} label="Cycle" value={stats.generationCycle} color="text-blue-300" />
      </div>

      {/* Evolution progress bar */}
      <div className="w-52 glass-pill p-2 rounded-xl">
        <div className="mb-1 flex justify-between text-[9px] font-semibold uppercase tracking-wider text-neutral-400">
          <span>Neural Evolution</span>
          <span className="text-cyan-300 font-mono">{Math.round(tier * 100)}%</span>
        </div>
        <div className="h-1.5 overflow-hidden rounded-full bg-neutral-900/80 ring-1 ring-white/5">
          <div
            className="h-full rounded-full bg-gradient-to-r from-emerald-400 via-cyan-400 to-violet-500 transition-all duration-1000 shadow-sm shadow-cyan-500/50"
            style={{ width: `${Math.max(4, tier * 100)}%` }}
          />
        </div>
      </div>
    </div>
  );
}

function StatChip({ icon, label, value, color }: { icon: React.ReactNode; label: string; value: number | string; color: string }) {
  return (
    <div className="flex items-center gap-1.5 rounded-lg glass-pill px-2.5 py-1">
      <span className={color}>{icon}</span>
      <span className="text-[10px] font-medium text-neutral-400">{label}</span>
      <span className={`text-[10px] font-bold font-mono ${color}`}>{value}</span>
    </div>
  );
}
