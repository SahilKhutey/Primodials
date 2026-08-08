import { X, Dna, Brain, Zap, Shield, Crown } from 'lucide-react';
import type { Organism } from '@/sim/types';

type Props = {
  org: Organism;
  onClose: () => void;
};

export function AmbientOrganismCard({ org, onClose }: Props) {
  const isCarnivore = org.genome.diet >= 0.5;
  const rank = org.socialRank ?? 'solitary';

  const rankBadgeStyle =
    rank === 'alpha'
      ? 'bg-amber-500/20 text-amber-300 ring-amber-400/40'
      : rank === 'beta'
      ? 'bg-cyan-500/20 text-cyan-300 ring-cyan-400/40'
      : rank === 'omega'
      ? 'bg-purple-500/20 text-purple-300 ring-purple-400/40'
      : 'bg-neutral-800 text-neutral-400 ring-neutral-700';

  return (
    <div className="fixed bottom-20 right-6 z-40 w-72 rounded-2xl border border-white/10 bg-neutral-950/85 p-4 shadow-2xl shadow-black/80 backdrop-blur-2xl text-neutral-200 transition-all duration-300">
      {/* Header */}
      <div className="flex items-center justify-between border-b border-white/10 pb-2.5">
        <div className="flex items-center gap-2">
          <div
            className="h-3.5 w-3.5 rounded-full ring-2 ring-white/20"
            style={{ backgroundColor: `hsl(${org.genome.hue}, 80%, 55%)` }}
          />
          <div>
            <h3 className="text-xs font-bold text-neutral-100">Organism #{org.id}</h3>
            <p className="text-[10px] text-neutral-400">
              Species #{org.speciesId} · Gen {org.generation}
            </p>
          </div>
        </div>
        <button
          onClick={onClose}
          className="rounded-lg p-1 text-neutral-400 hover:bg-white/10 hover:text-white transition"
        >
          <X size={14} />
        </button>
      </div>

      {/* Rank & Role Badges */}
      <div className="flex items-center gap-1.5 py-2.5">
        <span className={`flex items-center gap-1 rounded-full px-2.5 py-0.5 text-[10px] font-bold uppercase tracking-wider ring-1 ${rankBadgeStyle}`}>
          {rank === 'alpha' && <Crown size={11} />}
          {rank}
        </span>
        <span className="rounded-full bg-white/5 px-2.5 py-0.5 text-[10px] font-medium text-neutral-300 ring-1 ring-white/10 capitalize">
          {org.colonyRole ?? 'solitary'}
        </span>
        {org.inBiofilm && (
          <span className="rounded-full bg-emerald-500/20 px-2 py-0.5 text-[10px] font-medium text-emerald-300 ring-1 ring-emerald-400/30">
            Biofilm
          </span>
        )}
      </div>

      {/* Stats Grid */}
      <div className="space-y-2 text-xs">
        {/* Energy Bar */}
        <div>
          <div className="flex justify-between text-[11px] mb-1">
            <span className="text-neutral-400 flex items-center gap-1">
              <Zap size={11} className="text-amber-400" /> Energy
            </span>
            <span className="font-mono text-neutral-200">{Math.round(org.energy)} / 150</span>
          </div>
          <div className="h-1.5 w-full rounded-full bg-neutral-800 overflow-hidden">
            <div
              className="h-full rounded-full bg-gradient-to-r from-amber-500 to-emerald-400 transition-all duration-300"
              style={{ width: `${Math.min(100, (org.energy / 150) * 100)}%` }}
            />
          </div>
        </div>

        <div className="grid grid-cols-2 gap-2 pt-1 text-[11px]">
          <div className="rounded-xl bg-white/5 p-2 ring-1 ring-white/5">
            <div className="text-neutral-400 text-[10px]">Diet</div>
            <div className="font-medium text-neutral-200">{isCarnivore ? 'Carnivore 🥩' : 'Herbivore 🌿'}</div>
          </div>
          <div className="rounded-xl bg-white/5 p-2 ring-1 ring-white/5">
            <div className="text-neutral-400 text-[10px] flex items-center gap-1">
              <Brain size={10} className="text-violet-400" /> Intelligence
            </div>
            <div className="font-mono font-semibold text-violet-300">
              {(org.genome.intelligence * 100).toFixed(0)}%
            </div>
          </div>
          <div className="rounded-xl bg-white/5 p-2 ring-1 ring-white/5">
            <div className="text-neutral-400 text-[10px]">Speed / Sense</div>
            <div className="font-mono text-neutral-200">
              {org.genome.speed.toFixed(1)} / {Math.round(org.genome.senseRadius)}px
            </div>
          </div>
          <div className="rounded-xl bg-white/5 p-2 ring-1 ring-white/5">
            <div className="text-neutral-400 text-[10px] flex items-center gap-1">
              <Dna size={10} className="text-cyan-400" /> Cooperation
            </div>
            <div className="font-mono text-cyan-300">
              {(org.genome.cooperation * 100).toFixed(0)}%
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
