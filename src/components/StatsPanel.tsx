import { Activity, Heart, Skull, Swords, Dna, Leaf, Beef, Sparkles, Users, Baby, Brain, Trophy, Home, Hammer, Package, Lightbulb, Compass, Mountain, Clock, Globe, Zap, FlaskConical, Beaker, Layers, Shield, Share2, Sun, Bug, Infinity as InfinityIcon, UsersRound, Network, Handshake, Crown } from 'lucide-react';
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
      <StatCard icon={<Clock size={18} className="text-amber-300" />} label="Gen Cycle" value={sim.stats.generationCycle} color="bg-amber-950/50" />
      <StatCard icon={<Globe size={18} className="text-emerald-400" />} label="World Expansions" value={sim.stats.worldExpansions} color="bg-emerald-950/50" />
      <StatCard icon={<Brain size={18} className="text-fuchsia-400" />} label="Neural Brains" value={sim.stats.brainDrivenPopulation} color="bg-fuchsia-950/50" />
      <StatCard icon={<Zap size={18} className="text-yellow-400" />} label="Avg Fitness" value={sim.stats.avgFitness.toFixed(1)} color="bg-yellow-950/50" />
      <StatCard icon={<Trophy size={18} className="text-amber-400" />} label="Peak Fitness" value={sim.stats.peakFitness.toFixed(1)} color="bg-amber-950/50" />
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

      {/* Microbial evolution stats */}
      <div className="col-span-2 mt-1 text-xs font-semibold uppercase tracking-wider text-green-400 sm:col-span-3 lg:col-span-4">Microbial Evolution</div>
      <StatCard icon={<FlaskConical size={18} className="text-lime-400" />} label="Avg Chemotaxis" value={sim.stats.avgChemotaxis.toFixed(2)} color="bg-lime-950/50" />
      <StatCard icon={<Share2 size={18} className="text-green-400" />} label="Avg Quorum" value={sim.stats.avgQuorum.toFixed(2)} color="bg-green-950/50" />
      <StatCard icon={<Layers size={18} className="text-emerald-400" />} label="Avg Biofilm" value={sim.stats.avgBiofilm.toFixed(2)} color="bg-emerald-950/50" />
      <StatCard icon={<Beaker size={18} className="text-teal-400" />} label="Biofilm Clusters" value={sim.stats.biofilmClusters} color="bg-teal-950/50" />
      <StatCard icon={<Layers size={18} className="text-emerald-300" />} label="Biofilm Pop" value={sim.stats.biofilmPopulation} color="bg-emerald-950/50" />
      <StatCard icon={<Shield size={18} className="text-amber-300" />} label="Spores" value={sim.stats.spores} color="bg-amber-950/50" />
      <StatCard icon={<Share2 size={18} className="text-cyan-400" />} label="HGT Events" value={sim.stats.hgtEvents} color="bg-cyan-950/50" />

      {/* Advanced biological stats */}
      <div className="col-span-2 mt-1 text-xs font-semibold uppercase tracking-wider text-emerald-400 sm:col-span-3 lg:col-span-4">Advanced Biology</div>
      <StatCard icon={<Sun size={18} className="text-yellow-400" />} label="Photosynth" value={sim.stats.photosynthesizers} color="bg-yellow-950/50" />
      <StatCard icon={<Share2 size={18} className="text-emerald-300" />} label="Symbiotic Pairs" value={sim.stats.symbioticPairs} color="bg-emerald-950/50" />
      <StatCard icon={<Bug size={18} className="text-red-400" />} label="Parasitic Events" value={sim.stats.parasiticEvents} color="bg-red-950/50" />
      <StatCard icon={<Shield size={18} className="text-red-300" />} label="Infected" value={sim.stats.infected} color="bg-red-950/50" />
      <StatCard icon={<Shield size={18} className="text-orange-400" />} label="Disease Events" value={sim.stats.diseaseEvents} color="bg-orange-950/50" />
      <StatCard icon={<Sun size={18} className="text-amber-300" />} label="Avg Photo" value={sim.stats.avgPhotosynthesis.toFixed(2)} color="bg-amber-950/50" />
      <StatCard icon={<Leaf size={18} className="text-green-300" />} label="Avg Symbiosis" value={sim.stats.avgSymbiosis.toFixed(2)} color="bg-green-950/50" />
      <StatCard icon={<InfinityIcon size={18} className="text-amber-400" />} label="New Species" value={sim.stats.newSpeciesIntroduced} color="bg-amber-950/50" />

      {/* Social & ecological stats */}
      <div className="col-span-2 mt-1 text-xs font-semibold uppercase tracking-wider text-indigo-400 sm:col-span-3 lg:col-span-4">Social & Ecology</div>
      <StatCard icon={<Network size={18} className="text-indigo-400" />} label="Spatial Clusters" value={sim.stats.spatialClusters} color="bg-indigo-950/50" />
      <StatCard icon={<UsersRound size={18} className="text-indigo-300" />} label="Clustered Pop" value={sim.stats.clusteredPopulation} color="bg-indigo-950/50" />
      <StatCard icon={<Crown size={18} className="text-amber-400" />} label="Alphas" value={sim.stats.alphaCount} color="bg-amber-950/50" />
      <StatCard icon={<Handshake size={18} className="text-green-400" />} label="Altruism Events" value={sim.stats.altruismEvents} color="bg-green-950/50" />
      <StatCard icon={<Swords size={18} className="text-orange-400" />} label="Competition Events" value={sim.stats.competitionEvents} color="bg-orange-950/50" />
      <StatCard icon={<Network size={18} className="text-slate-300" />} label="Neutral Drift" value={sim.stats.neutralDriftEvents} color="bg-slate-800/50" />
      <StatCard icon={<UsersRound size={18} className="text-indigo-300" />} label="Avg Clustering" value={sim.stats.avgClustering.toFixed(2)} color="bg-indigo-950/50" />
      <StatCard icon={<Handshake size={18} className="text-green-300" />} label="Avg Altruism" value={sim.stats.avgAltruism.toFixed(2)} color="bg-green-950/50" />
      <StatCard icon={<Crown size={18} className="text-amber-300" />} label="Avg Dominance" value={sim.stats.avgDominance.toFixed(2)} color="bg-amber-950/50" />
      <StatCard icon={<Mountain size={18} className="text-teal-300" />} label="Avg Niche Width" value={sim.stats.avgNicheBreadth.toFixed(2)} color="bg-teal-950/50" />
      <StatCard icon={<Swords size={18} className="text-orange-300" />} label="Avg Competitiveness" value={sim.stats.avgCompetitiveness.toFixed(2)} color="bg-orange-950/50" />
    </div>
  );
}
