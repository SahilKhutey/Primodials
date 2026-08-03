import { Users, Trophy, Brain, Swords, Home, Lightbulb, Sparkles } from 'lucide-react';
import type { Species, Colony, Structure } from '@/sim/types';
import { intelligenceTier, civilizationLevel } from '@/sim/types';

type Props = {
  species: Species[];
  colonies: Colony[];
  structures: Structure[];
};

const SHAPE_NAMES: Record<number, string> = {
  3: 'Triangle', 4: 'Square', 5: 'Pentagon',
  6: 'Hexagon', 7: 'Heptagon', 8: 'Octagon',
};

function rankLabel(rank: number, total: number): { text: string; color: string } {
  if (rank === 1) return { text: 'Apex — Dominant', color: 'text-amber-300' };
  if (rank <= Math.ceil(total * 0.25)) return { text: 'Predator tier', color: 'text-orange-300' };
  if (rank <= Math.ceil(total * 0.5)) return { text: 'Mid-tier', color: 'text-neutral-300' };
  return { text: 'Prey tier', color: 'text-neutral-500' };
}

export function SpeciesPanel({ species, colonies, structures }: Props) {
  const sorted = [...species].sort((a, b) => a.rank - b.rank);
  const coloniesBySpecies = new Map<number, Colony[]>();
  for (const c of colonies) {
    const arr = coloniesBySpecies.get(c.speciesId) ?? [];
    arr.push(c);
    coloniesBySpecies.set(c.speciesId, arr);
  }
  const structuresBySpecies = new Map<number, Structure[]>();
  for (const s of structures) {
    const arr = structuresBySpecies.get(s.speciesId) ?? [];
    arr.push(s);
    structuresBySpecies.set(s.speciesId, arr);
  }

  return (
    <div className="space-y-2 max-h-[28rem] overflow-y-auto pr-1">
      {sorted.length === 0 && <p className="text-sm text-neutral-500 italic">No living species.</p>}
      {sorted.map((sp) => {
        const spColonies = coloniesBySpecies.get(sp.id) ?? [];
        const spStructures = structuresBySpecies.get(sp.id) ?? [];
        const tier = intelligenceTier(sp.avgIntelligence);
        const rank = rankLabel(sp.rank, sorted.length);
        const civ = civilizationLevel(spStructures.length);
        return (
          <div key={sp.id} className="rounded-lg border border-neutral-700/50 bg-neutral-800/40 p-2.5">
            <div className="mb-2 flex items-center gap-1.5">
              <span className="text-xs font-bold text-neutral-400">#{sp.rank}</span>
              <span className={`text-xs font-medium ${rank.color}`}>{rank.text}</span>
              {sp.rank === 1 && sp.totalKills > 0 && (
                <span className="ml-auto flex items-center gap-1 text-xs text-amber-400"><Trophy size={11} />{sp.totalKills} kills</span>
              )}
            </div>

            <div className="flex items-center gap-3">
              <div className="h-8 w-8 shrink-0 rounded-md border border-white/20" style={{ backgroundColor: `hsl(${sp.hue}, 22%, 45%)` }} />
              <div className="flex-1 min-w-0">
                <div className="text-sm font-medium text-neutral-200">Species #{sp.id}</div>
                <div className="text-xs text-neutral-400">
                  {SHAPE_NAMES[sp.representative.sides] ?? `${sp.representative.sides}-gon`} ·{' '}
                  {sp.representative.size.toFixed(1)}px ·{' '}
                  {sp.representative.diet >= 0.5 ? 'Carnivore' : 'Herbivore'}
                </div>
              </div>
              <div className="text-right">
                <div className="text-sm font-semibold text-neutral-100">{sp.count}</div>
                <div className="text-xs text-neutral-500">alive</div>
              </div>
            </div>

            <div className="mt-2 flex flex-wrap items-center gap-2 border-t border-neutral-700/40 pt-2">
              <span className="inline-flex items-center gap-1 rounded-full bg-neutral-700/40 px-2 py-0.5 text-xs text-neutral-300">
                <Brain size={11} />{tier.label}<span className="font-mono text-neutral-500">{sp.avgIntelligence.toFixed(2)}</span>
              </span>
              {sp.totalKills > 0 && (
                <span className="inline-flex items-center gap-1 rounded-full bg-red-950/30 px-2 py-0.5 text-xs text-red-300"><Swords size={11} />{sp.totalKills} kills</span>
              )}
              {spColonies.length > 0 && (
                <span className="inline-flex items-center gap-1 rounded-full bg-teal-950/30 px-2 py-0.5 text-xs text-teal-300"><Users size={11} />{spColonies.length} {spColonies.length === 1 ? 'colony' : 'colonies'}</span>
              )}
              {spStructures.length > 0 && (
                <span className="inline-flex items-center gap-1 rounded-full bg-sky-950/30 px-2 py-0.5 text-xs text-sky-300"><Home size={11} />{spStructures.length} structures</span>
              )}
              {sp.knowledgeDiscovered > 0 && (
                <span className="inline-flex items-center gap-1 rounded-full bg-indigo-950/30 px-2 py-0.5 text-xs text-indigo-300"><Lightbulb size={11} />{sp.knowledgeDiscovered} knowledge</span>
              )}
              {sp.evolutionLeaps > 0 && (
                <span className="inline-flex items-center gap-1 rounded-full bg-violet-950/30 px-2 py-0.5 text-xs text-violet-300"><Sparkles size={11} />{sp.evolutionLeaps} leaps</span>
              )}
            </div>

            {spStructures.length > 0 && (
              <div className="mt-1.5 flex items-center gap-1.5 text-xs">
                <span className="text-neutral-500">Civ:</span>
                <span className="font-medium text-neutral-300">{civ.label}</span>
              </div>
            )}
          </div>
        );
      })}
    </div>
  );
}
