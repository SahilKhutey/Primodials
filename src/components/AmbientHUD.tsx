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
  const [opacity, setOpacity] = useState(0.7);
  const lastInteraction = useRef(Date.now());

  useEffect(() => {
    const onMove = () => {
      lastInteraction.current = Date.now();
      setOpacity(1);
    };
    window.addEventListener('mousemove', onMove);
    return () => window.removeEventListener('mousemove', onMove);
  }, []);

  useEffect(() => {
    const id = setInterval(() => {
      if (Date.now() - lastInteraction.current > 3000) {
        setOpacity(0.25);
      } else {
        setOpacity(1);
      }
    }, 500);
    return () => clearInterval(id);
  }, []);

  if (!visible) return null;

  const stats = sim.stats;
  const tier = stats.avgIntelligence;

  return (
    <div
      className="pointer-events-none fixed left-6 top-6 z-30 flex flex-col gap-2 transition-opacity duration-1000"
      style={{ opacity }}
    >
      {/* Title */}
      <div className="flex items-center gap-2.5">
        <div className="relative">
          <div className="absolute inset-0 rounded-lg bg-gradient-to-br from-cyan-500/20 to-emerald-500/10 blur-sm" />
          <div className="relative rounded-lg bg-neutral-900/60 p-1.5 ring-1 ring-white/10 backdrop-blur-md">
            <Dna className="text-cyan-300" size={18} />
          </div>
        </div>
        <div>
          <div className="text-sm font-bold tracking-tight text-neutral-100">Primordials</div>
          <div className="text-[10px] text-neutral-500">Living wallpaper engine</div>
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
      <div className="w-48">
        <div className="mb-0.5 flex justify-between text-[9px] font-medium uppercase tracking-wider text-neutral-500">
          <span>Evolution</span>
          <span>{Math.round(tier * 100)}%</span>
        </div>
        <div className="h-1 overflow-hidden rounded-full bg-neutral-800/60">
          <div
            className="h-full rounded-full bg-gradient-to-r from-emerald-500 via-cyan-500 to-violet-500 transition-all duration-1000"
            style={{ width: `${tier * 100}%` }}
          />
        </div>
      </div>
    </div>
  );
}

function StatChip({ icon, label, value, color }: { icon: React.ReactNode; label: string; value: number | string; color: string }) {
  return (
    <div className="flex items-center gap-1 rounded-md bg-neutral-900/50 px-2 py-1 ring-1 ring-white/5 backdrop-blur-md">
      <span className={color}>{icon}</span>
      <span className="text-[10px] font-semibold text-neutral-400">{label}</span>
      <span className={`text-[10px] font-bold ${color}`}>{value}</span>
    </div>
  );
}
