import { Activity, Heart, Skull, Swords, Dna, Leaf, Beef, Sparkles, Users, Baby, Brain, Trophy, Home, Hammer, Package, Lightbulb, Compass, Mountain } from 'lucide-react';
import type { Simulation } from '@/sim/simulation';

type Props = { sim: Simulation };

function StatCard({ icon, label, value, color }: { icon: React.ReactNode; label: string; value: string | number; color: string }) {
  return (
    <div className="flex items-center gap-3 rounded-xl border border-neutral-700/60 bg-neutral-800/50 p-3">
      <div className={`rounded-lg p-2 ${color}`}>{icon}</div>
      <div>
        <div className="text-xs uppercase tracking-wider text-neutral-400">{label}</div>
        <div className="text-lg font-semibold text-neutral-100">{value}</div>
      </div>
    </div>
  );
}

export function StatsPanel({ sim }: Props) {
  const dominantSpecies = sim.species.find((s) => s.id === sim.stats.dominantSpeciesId) ?? null;

  return (
    <div className="grid grid-cols-2 gap-2 sm:grid-cols-3 lg:grid-cols-4">
      <StatCard icon={<Activity size={18} className="text-neutral-300" />} label="Tick" value={sim.tick} color="bg-neutral-700/50" />
      <StatCard icon={<Dna size={18} className="text-violet-400" />} label="Species" value={sim.speciesCount} color="bg-violet-950/50" />
      <StatCard icon={<Heart size={18} className="text-rose-400" />} label="Population" value={sim.population} color="bg-rose-950/50" />
      <StatCard icon={<Sparkles size={18} className="text-amber-400" />} label="Generations" value={sim.generations} color="bg-amber-950/50" />
      <StatCard icon={<Brain size={18} className="text-neutral-200" />} label="Avg Intelligence" value={sim.stats.avgIntelligence.toFixed(2)} color="bg-neutral-700/50" />
      <StatCard icon={<Hammer size={18} className="text-neutral-300" />} label="Avg Build Skill" value={sim.stats.avgBuildSkill.toFixed(2)} color="bg-neutral-700/50" />
      <StatCard icon={<Compass size={18} className="text-cyan-400" />} label="Avg Curiosity" value={sim.stats.avgCuriosity.toFixed(2)} color="bg-cyan-950/50" />
      <StatCard icon={<Mountain size={18} className="text-amber-400" />} label="Avg Adaptability" value={sim.stats.avgAdaptability.toFixed(2)} color="bg-amber-950/50" />
      <StatCard icon={<Trophy size={18} className="text-amber-400" />} label="Dominant" value={dominantSpecies ? `#${dominantSpecies.id}` : '—'} color="bg-amber-950/50" />
      <StatCard icon={<Home size={18} className="text-sky-400" />} label="Structures" value={sim.stats.structures} color="bg-sky-950/50" />
      <StatCard icon={<Package size={18} className="text-orange-400" />} label="Remains" value={sim.stats.remains} color="bg-orange-950/50" />
      <StatCard icon={<Lightbulb size={18} className="text-indigo-300" />} label="Knowledge Found" value={sim.stats.knowledgeDiscovered} color="bg-indigo-950/50" />
      <StatCard icon={<Sparkles size={18} className="text-violet-300" />} label="Evo Leaps" value={sim.stats.evolutionLeaps} color="bg-violet-950/50" />
      <StatCard icon={<Leaf size={18} className="text-emerald-400" />} label="Herbivores" value={sim.stats.herbivores} color="bg-emerald-950/50" />
      <StatCard icon={<Beef size={18} className="text-red-400" />} label="Carnivores" value={sim.stats.carnivores} color="bg-red-950/50" />
      <StatCard icon={<Users size={18} className="text-teal-400" />} label="Colonies" value={sim.stats.colonies} color="bg-teal-950/50" />
      <StatCard icon={<Baby size={18} className="text-pink-400" />} label="Births" value={sim.stats.births} color="bg-pink-950/50" />
      <StatCard icon={<Skull size={18} className="text-neutral-400" />} label="Deaths" value={sim.stats.deaths} color="bg-neutral-700/50" />
      <StatCard icon={<Swords size={18} className="text-orange-400" />} label="Kills" value={sim.stats.kills} color="bg-orange-950/50" />
    </div>
  );
}
