import { useEffect, useState } from 'react';
import { Hexagon, Leaf, Beef, Dna, Sparkles, X } from 'lucide-react';
import type { Simulation } from '@/sim/simulation';

type Props = {
  sim: Simulation;
  selectedId: number | null;
  onClose: () => void;
};

const SHAPE_NAMES: Record<number, string> = {
  3: 'Triangle', 4: 'Square', 5: 'Pentagon',
  6: 'Hexagon', 7: 'Heptagon', 8: 'Octagon',
};

function dietLabel(diet: number): string {
  return diet >= 0.5 ? 'Carnivore' : 'Herbivore';
}

function intelligenceLabel(intel: number): string {
  if (intel >= 0.8) return 'Brilliant';
  if (intel >= 0.6) return 'Sharp';
  if (intel >= 0.4) return 'Aware';
  if (intel >= 0.2) return 'Instinctive';
  return 'Simple';
}

// A minimal, ambient-styled "learn something" card for Wallpaper Mode.
// Deliberately shows only a handful of facts — this is a glance, not the
// full Inspector. Matches the glassmorphic language used by AmbientHUD /
// WallpaperDock (bg-neutral-950/70, backdrop-blur-xl, ring-1 ring-white/10),
// and uses a state-driven mount transition (core Tailwind utilities only —
// no animate-in plugin is registered in this project's tailwind.config.js).
export function WallpaperInfoPopover({ sim, selectedId, onClose }: Props) {
  const [mounted, setMounted] = useState(false);

  useEffect(() => {
    if (selectedId === null) {
      setMounted(false);
      return;
    }
    const raf = requestAnimationFrame(() => setMounted(true));
    return () => cancelAnimationFrame(raf);
  }, [selectedId]);

  if (selectedId === null) return null;

  const org = sim.organisms.find((o) => o.id === selectedId && o.alive);
  if (!org) return null;

  const g = org.genome;
  const species = sim.species.find((s) => s.id === org.speciesId);
  const shapeName = SHAPE_NAMES[g.sides] ?? `${g.sides}-gon`;
  const isCarnivore = g.diet >= 0.5;

  return (
    <div
      className={`pointer-events-auto fixed bottom-6 right-6 z-30 w-64 transition-all duration-300 ease-out ${
        mounted ? 'translate-y-0 opacity-100' : 'translate-y-2 opacity-0'
      }`}
    >
      <div className="rounded-2xl border border-white/10 bg-neutral-950/70 p-4 shadow-2xl shadow-black/60 backdrop-blur-xl">
        <div className="mb-3 flex items-start justify-between">
          <div className="flex items-center gap-2">
            <div
              className="flex h-8 w-8 items-center justify-center rounded-lg ring-1 ring-white/10"
              style={{ background: `hsla(${g.hue}, 65%, 45%, 0.25)` }}
            >
              <Hexagon size={16} style={{ color: `hsl(${g.hue}, 70%, 65%)` }} />
            </div>
            <div>
              <div className="text-sm font-bold text-neutral-100">
                Species #{org.speciesId}
              </div>
              <div className="text-[10px] text-neutral-500">{shapeName}</div>
            </div>
          </div>
          <button
            onClick={onClose}
            className="rounded-md p-1 text-neutral-500 transition hover:bg-white/10 hover:text-neutral-200"
            aria-label="Close"
          >
            <X size={14} />
          </button>
        </div>

        <div className="grid grid-cols-2 gap-1.5">
          <Fact
            icon={isCarnivore ? <Beef size={12} /> : <Leaf size={12} />}
            label="Diet"
            value={dietLabel(g.diet)}
            color={isCarnivore ? 'text-red-300' : 'text-emerald-300'}
          />
          <Fact
            icon={<Dna size={12} />}
            label="Generation"
            value={org.generation}
            color="text-amber-300"
          />
          <Fact
            icon={<Sparkles size={12} />}
            label="Mind"
            value={intelligenceLabel(g.intelligence)}
            color="text-violet-300"
          />
          <Fact
            icon={<Hexagon size={12} />}
            label="Population"
            value={species?.count ?? 1}
            color="text-cyan-300"
          />
        </div>
      </div>
    </div>
  );
}

function Fact({ icon, label, value, color }: { icon: React.ReactNode; label: string; value: number | string; color: string }) {
  return (
    <div className="flex items-center gap-1.5 rounded-lg bg-neutral-900/50 px-2 py-1.5 ring-1 ring-white/5">
      <span className={color}>{icon}</span>
      <div className="leading-tight">
        <div className="text-[9px] font-medium uppercase tracking-wide text-neutral-500">{label}</div>
        <div className={`text-[11px] font-bold ${color}`}>{value}</div>
      </div>
    </div>
  );
}
