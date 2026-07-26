import { useMemo } from 'react';
import {
  Hexagon, Leaf, Beef, Gauge, Radar, Flame, Heart, Clock, Dna,
  Target, Zap, TrendingUp, X, Info, Users, Swords, Handshake,
  Sparkles, Crown, Baby, Brain, Trophy, AlertTriangle, Hammer, Home,
  Compass, Lightbulb, Package, Mountain, TreePine, Sun, Droplets, Snowflake,
} from 'lucide-react';
import type { Organism, Genome, BiomeType } from '@/sim/types';
import { intelligenceTier, civilizationLevel, BIOME_INFO } from '@/sim/types';
import type { Simulation } from '@/sim/simulation';

type Props = {
  sim: Simulation;
  selectedId: number | null;
  onClear: () => void;
};

const SHAPE_NAMES: Record<number, string> = {
  3: 'Triangle', 4: 'Square', 5: 'Pentagon',
  6: 'Hexagon', 7: 'Heptagon', 8: 'Octagon',
};

function dietLabel(diet: number): string { return diet >= 0.5 ? 'Carnivore' : 'Herbivore'; }
function lifespanEstimate(genome: Genome): string { return `${Math.round(genome.lifespan)} ticks`; }
function energyPerTick(genome: Genome): string {
  const cost = (genome.size * 0.01 + genome.speed * 0.04 + genome.senseRadius * 0.002) * genome.metabolism;
  return cost.toFixed(2);
}

function goalText(org: Organism): string {
  const isCarnivore = org.genome.diet >= 0.5;
  if (org.age > org.genome.lifespan) return 'Dying of old age';
  if (org.threatLevel > 0.3) return 'Fleeing from a predator';
  if (org.energy < 50) return isCarnivore ? 'Starving — hunting desperately' : 'Starving — seeking food urgently';
  if (org.knowledgeBoost > 0.05) return 'Enlightened — boosted by discovered knowledge';
  if (org.carrying > 0 && org.colonyRole === 'leader') return 'Carrying materials to build';
  if (org.energy > 120 && org.reproductionCooldown <= 0)
    return org.genome.cooperation > 0.5 ? 'Seeking a mate to reproduce' : 'Ready to split and reproduce';
  if (org.buildCooldown > 100 && org.colonyRole === 'leader') return 'Building a structure';
  if (org.colonyRole === 'leader') return 'Leading its colony';
  if (org.colonyRole === 'member') return 'Following the colony';
  if (org.genome.curiosity > 0.5 && org.genome.intelligence >= 0.4) return 'Exploring for knowledge';
  if (org.genome.intelligence >= 0.5 && org.genome.buildSkill >= 0.3 && org.carrying < 15) return 'Gathering dead matter for building';
  if (isCarnivore) return 'Stalking prey';
  return 'Grazing peacefully';
}

function traitLevel(v: number): string {
  if (v < 0.25) return 'Low';
  if (v < 0.5) return 'Moderate';
  if (v < 0.75) return 'High';
  return 'Very High';
}

function cognitiveAbilities(intel: number): string[] {
  const abilities: string[] = [];
  if (intel >= 0.2) abilities.push('Threat detection');
  if (intel >= 0.4) abilities.push('Preferential foraging');
  if (intel >= 0.4) abilities.push('Structure building');
  if (intel >= 0.5) abilities.push('Material gathering');
  if (intel >= 0.5) abilities.push('Strategic fleeing');
  if (intel >= 0.6) abilities.push('Target prioritization');
  if (intel >= 0.6) abilities.push('Watchtower construction');
  if (intel >= 0.7) abilities.push('Wall construction');
  if (intel >= 0.8) abilities.push('Coordinated pack tactics');
  return abilities;
}

const BIOME_ICONS: Record<BiomeType, React.ReactNode> = {
  plains: <Sun size={12} className="text-yellow-400" />,
  forest: <TreePine size={12} className="text-green-400" />,
  desert: <Sun size={12} className="text-orange-400" />,
  tundra: <Snowflake size={12} className="text-blue-300" />,
  wetland: <Droplets size={12} className="text-cyan-400" />,
};

export function InspectorPanel({ sim, selectedId, onClear }: Props) {
  const selected = useMemo(
    () => sim.organisms.find((o) => o.id === selectedId && o.alive) ?? null,
    [sim, selectedId, sim.tick],
  );

  const speciesOf = useMemo(() => {
    if (!selected) return null;
    return sim.species.find((s) => s.id === selected.speciesId) ?? null;
  }, [sim, selected]);

  const colonyOf = useMemo(() => {
    if (!selected || selected.colonyId === null) return null;
    return sim.colonies.find((c) => c.id === selected.colonyId) ?? null;
  }, [sim, selected]);

  const colonyStructures = useMemo(() => {
    if (!selected || selected.colonyId === null) return [];
    return sim.structures.filter((s) => s.colonyId === selected.colonyId);
  }, [sim, selected]);

  if (!selected) {
    return (
      <div className="flex flex-col items-center justify-center gap-3 py-10 text-center">
        <div className="rounded-full bg-neutral-800 p-4">
          <Info className="text-neutral-400" size={28} />
        </div>
        <p className="text-sm text-neutral-400">
          Click any creature in the simulation to inspect its identity, attributes, intelligence, goals, and lifespan.
        </p>
      </div>
    );
  }

  const g = selected.genome;
  const isCarnivore = g.diet >= 0.5;
  const shapeName = SHAPE_NAMES[g.sides] ?? `${g.sides}-gon`;
  const effectiveIntel = Math.min(1, g.intelligence + selected.knowledgeBoost);
  const tier = intelligenceTier(effectiveIntel);
  const abilities = cognitiveAbilities(effectiveIntel);
  const civLevel = speciesOf ? civilizationLevel(speciesOf.civilizationLevel) : null;
  const biome = selected.biomeId ? sim.biomes.find((b) => b.id === selected.biomeId) : null;
  const biomeInfo = biome ? BIOME_INFO[biome.type] : null;

  return (
    <div className="space-y-4">
      {/* Identity header */}
      <div className="flex items-start gap-3 rounded-xl border border-neutral-700/60 bg-neutral-800/50 p-3">
        <div
          className="flex h-14 w-14 shrink-0 items-center justify-center rounded-lg border border-white/20"
          style={{ backgroundColor: `hsl(${g.hue}, 22%, 45%)` }}
        >
          <Hexagon className="text-white/90" size={28} />
        </div>
        <div className="flex-1 min-w-0">
          <div className="flex items-center gap-2">
            <h3 className="text-base font-bold text-neutral-100">Organism #{selected.id}</h3>
            <button onClick={onClear} className="ml-auto rounded-md p-1 text-neutral-500 transition hover:bg-neutral-700 hover:text-neutral-200">
              <X size={15} />
            </button>
          </div>
          <div className="mt-0.5 flex flex-wrap gap-1.5">
            <Badge color="bg-neutral-700 text-neutral-200">{shapeName}</Badge>
            <Badge color={isCarnivore ? 'bg-red-950/60 text-red-300' : 'bg-emerald-950/60 text-emerald-300'}>
              {isCarnivore ? <Beef size={11} /> : <Leaf size={11} />}
              {dietLabel(g.diet)}
            </Badge>
            {speciesOf && (
              <Badge color="bg-violet-950/60 text-violet-300"><Dna size={11} />Species #{speciesOf.id}</Badge>
            )}
            {speciesOf && (
              <Badge color={speciesOf.rank === 1 ? 'bg-amber-950/60 text-amber-300' : speciesOf.rank <= 3 ? 'bg-orange-950/50 text-orange-300' : 'bg-neutral-700/60 text-neutral-300'}>
                <Trophy size={11} />Rank #{speciesOf.rank}
              </Badge>
            )}
            {selected.colonyRole === 'leader' && (
              <Badge color="bg-amber-950/60 text-amber-300"><Crown size={11} />Colony Leader</Badge>
            )}
            {selected.colonyRole === 'member' && (
              <Badge color="bg-sky-950/60 text-sky-300"><Users size={11} />Colony Member</Badge>
            )}
          </div>
        </div>
      </div>

      {/* Goal */}
      <div className="rounded-xl border border-neutral-700/40 bg-neutral-800/30 p-3">
        <div className="flex items-center gap-2 text-xs uppercase tracking-wider text-neutral-400">
          <Target size={14} />Current Goal
        </div>
        <p className="mt-1 text-sm text-neutral-200">{goalText(selected)}</p>
        {selected.threatLevel > 0.3 && (
          <div className="mt-2 flex items-center gap-1.5 text-xs text-red-400">
            <AlertTriangle size={12} />Under threat — predator nearby
          </div>
        )}
      </div>

      {/* Intelligence */}
      <div className="rounded-xl border border-neutral-600/40 bg-neutral-800/40 p-3">
        <div className="flex items-center justify-between">
          <div className="flex items-center gap-2 text-xs uppercase tracking-wider text-neutral-400">
            <Brain size={14} />Intelligence
          </div>
          <span className="font-mono text-sm text-neutral-200">{effectiveIntel.toFixed(2)}{selected.knowledgeBoost > 0.01 && <span className="text-cyan-400"> (+{selected.knowledgeBoost.toFixed(2)})</span>}</span>
        </div>
        <div className="mt-2">
          <div className="h-2 overflow-hidden rounded-full bg-neutral-700/60">
            <div className="h-full rounded-full bg-gradient-to-r from-neutral-500 to-neutral-200 transition-all duration-300" style={{ width: `${effectiveIntel * 100}%` }} />
          </div>
        </div>
        <div className="mt-2 text-sm font-semibold text-neutral-100">{tier.label}</div>
        <p className="text-xs text-neutral-400">{tier.desc}</p>
        {abilities.length > 0 && (
          <div className="mt-2 flex flex-wrap gap-1.5">
            {abilities.map((a) => (
              <span key={a} className="rounded-full bg-neutral-700/50 px-2 py-0.5 text-xs text-neutral-300">{a}</span>
            ))}
          </div>
        )}
        {selected.knowledgeBoost > 0.01 && (
          <div className="mt-2 flex items-center gap-1.5 text-xs text-cyan-300">
            <Lightbulb size={12} />Knowledge boost: +{selected.knowledgeBoost.toFixed(2)} intelligence
          </div>
        )}
      </div>

      {/* Biome */}
      {biome && biomeInfo && (
        <div className="rounded-xl border border-neutral-700/40 bg-neutral-800/30 p-3">
          <div className="flex items-center gap-2 text-xs uppercase tracking-wider text-neutral-400">
            <Mountain size={14} />Biome
          </div>
          <div className="mt-1 flex items-center gap-1.5">
            {BIOME_ICONS[biome.type]}
            <span className="text-sm font-semibold text-neutral-100">{biomeInfo.label}</span>
          </div>
          <p className="text-xs text-neutral-400">{biomeInfo.desc}</p>
          <div className="mt-2 grid grid-cols-3 gap-1.5 text-xs">
            <div className="rounded-md bg-neutral-800/50 px-2 py-1">
              <span className="text-neutral-500">Food</span> <span className="font-mono text-neutral-300">{biomeInfo.foodRate.toFixed(1)}x</span>
            </div>
            <div className="rounded-md bg-neutral-800/50 px-2 py-1">
              <span className="text-neutral-500">Drain</span> <span className="font-mono text-neutral-300">{biomeInfo.energyDrain.toFixed(1)}x</span>
            </div>
            <div className="rounded-md bg-neutral-800/50 px-2 py-1">
              <span className="text-neutral-500">Speed</span> <span className="font-mono text-neutral-300">{biomeInfo.speedMod.toFixed(2)}x</span>
            </div>
          </div>
        </div>
      )}

      {/* Civilization */}
      {civLevel && (
        <div className="rounded-xl border border-neutral-700/40 bg-neutral-800/30 p-3">
          <div className="flex items-center gap-2 text-xs uppercase tracking-wider text-neutral-400">
            <Home size={14} />Civilization
          </div>
          <div className="mt-1 text-sm font-semibold text-neutral-100">{civLevel.label}</div>
          <p className="text-xs text-neutral-400">{civLevel.desc}</p>
          <div className="mt-2 flex flex-wrap gap-1.5">
            <Badge color="bg-neutral-700/50 text-neutral-300"><Hammer size={11} />Build skill: {traitLevel(g.buildSkill)}</Badge>
            {speciesOf && speciesOf.structuresBuilt > 0 && (
              <Badge color="bg-teal-950/40 text-teal-300"><Home size={11} />{speciesOf.structuresBuilt} built</Badge>
            )}
            {colonyStructures.length > 0 && (
              <Badge color="bg-sky-950/40 text-sky-300"><Home size={11} />{colonyStructures.length} in colony</Badge>
            )}
          </div>
          {colonyStructures.length > 0 && (
            <div className="mt-2 space-y-1">
              {colonyStructures.map((s) => (
                <div key={s.id} className="flex items-center justify-between rounded-md bg-neutral-800/50 px-2 py-1 text-xs">
                  <span className="text-neutral-300 capitalize">{s.type}</span>
                  <span className="font-mono text-neutral-500">HP {Math.round(s.hp)}/{s.maxHp}</span>
                </div>
              ))}
            </div>
          )}
        </div>
      )}

      {/* Attributes */}
      <div>
        <SectionTitle icon={<Zap size={14} />} text="Attributes" />
        <div className="grid grid-cols-2 gap-2">
          <AttrCard icon={<Hexagon size={15} className="text-neutral-300" />} label="Shape" value={shapeName} sub={`${g.sides} sides`} />
          <AttrCard icon={<Dna size={15} className="text-violet-400" />} label="Size" value={`${g.size.toFixed(1)} px`} sub="body radius" />
          <AttrCard icon={<Gauge size={15} className="text-cyan-400" />} label="Speed" value={g.speed.toFixed(2)} sub="px / tick" />
          <AttrCard icon={<Radar size={15} className="text-amber-400" />} label="Sense" value={`${g.senseRadius.toFixed(0)} px`} sub="perception range" />
          <AttrCard icon={<Flame size={15} className="text-orange-400" />} label="Metabolism" value={`${g.metabolism.toFixed(2)}x`} sub="energy cost mult." />
          <AttrCard icon={<Leaf size={15} className="text-emerald-400" />} label="Diet" value={dietLabel(g.diet)} sub={isCarnivore ? 'hunts prey' : 'eats food'} />
        </div>
      </div>

      {/* Social traits */}
      <div>
        <SectionTitle icon={<Users size={14} />} text="Social Traits" />
        <div className="grid grid-cols-2 gap-2">
          <AttrCard icon={<Swords size={15} className="text-red-400" />} label="Aggression" value={traitLevel(g.aggression)} sub={g.aggression.toFixed(2)} />
          <AttrCard icon={<Handshake size={15} className="text-sky-400" />} label="Cooperation" value={traitLevel(g.cooperation)} sub={g.cooperation.toFixed(2)} />
          <AttrCard icon={<Users size={15} className="text-teal-400" />} label="Social" value={traitLevel(g.socialGene)} sub={g.socialGene.toFixed(2)} />
          <AttrCard icon={<Baby size={15} className="text-pink-400" />} label="Fertility" value={traitLevel(g.fertility)} sub={g.fertility.toFixed(2)} />
          <AttrCard icon={<Compass size={15} className="text-cyan-400" />} label="Curiosity" value={traitLevel(g.curiosity)} sub={g.curiosity.toFixed(2)} />
          <AttrCard icon={<Mountain size={15} className="text-amber-400" />} label="Adaptability" value={traitLevel(g.adaptability)} sub={g.adaptability.toFixed(2)} />
        </div>
      </div>

      {/* Materials & Knowledge */}
      <div>
        <SectionTitle icon={<Package size={14} />} text="Materials &amp; Knowledge" />
        <div className="grid grid-cols-2 gap-2">
          <AttrCard icon={<Package size={15} className="text-amber-500" />} label="Carrying" value={`${selected.carrying} / 15`} sub="dead material" />
          <AttrCard icon={<Lightbulb size={15} className="text-cyan-400" />} label="Knowledge" value={selected.knowledgeBoost > 0.01 ? `+${selected.knowledgeBoost.toFixed(2)}` : 'None'} sub={selected.knowledgeBoost > 0.01 ? 'boost active' : 'undiscovered'} />
        </div>
      </div>

      {/* Vitals */}
      <div>
        <SectionTitle icon={<Heart size={14} />} text="Vitals" />
        <div className="space-y-2">
          <VitalBar label="Energy" value={selected.energy} max={200} color="bg-rose-500" icon={<Heart size={13} className="text-rose-400" />} />
          <VitalBar label="Age" value={selected.age} max={g.lifespan} color="bg-amber-500" icon={<Clock size={13} className="text-amber-400" />} />
          <VitalBar label="Reproduction cooldown" value={selected.reproductionCooldown} max={60} color="bg-cyan-500" icon={<Dna size={13} className="text-cyan-400" />} inverse />
        </div>
      </div>

      {/* Lifespan & energy */}
      <div>
        <SectionTitle icon={<Clock size={14} />} text="Lifespan &amp; Energy" />
        <div className="grid grid-cols-2 gap-2">
          <AttrCard icon={<Clock size={15} className="text-amber-400" />} label="Est. lifespan" value={lifespanEstimate(g)} sub="max age gene" />
          <AttrCard icon={<Flame size={15} className="text-orange-400" />} label="Energy cost" value={`${energyPerTick(g)}/tick`} sub="metabolic drain" />
        </div>
      </div>

      {/* Lineage & Colony */}
      <div>
        <SectionTitle icon={<TrendingUp size={14} />} text="Lineage &amp; Colony" />
        <div className="grid grid-cols-2 gap-2">
          <AttrCard icon={<Dna size={15} className="text-violet-400" />} label="Generation" value={selected.generation} sub="births since origin" />
          <AttrCard icon={<Hexagon size={15} className="text-neutral-300" />} label="Species pop." value={speciesOf?.count ?? 0} sub="same-species alive" />
          {speciesOf && (
            <AttrCard icon={<Trophy size={15} className="text-amber-400" />} label="Species rank" value={`#${speciesOf.rank}`} sub={speciesOf.rank === 1 ? 'apex / dominant' : speciesOf.rank <= 3 ? 'predator tier' : 'prey tier'} />
          )}
          {colonyOf ? (
            <>
              <AttrCard icon={<Users size={15} className="text-teal-400" />} label="Colony" value={`#${colonyOf.id}`} sub={`${colonyOf.members} members`} />
              <AttrCard icon={<Crown size={15} className="text-amber-400" />} label="Role" value={selected.colonyRole} sub={selected.colonyRole === 'leader' ? 'leads the group' : 'follows leader'} />
            </>
          ) : (
            <AttrCard icon={<Users size={15} className="text-neutral-500" />} label="Colony" value="Solitary" sub="lives alone" />
          )}
        </div>
      </div>
    </div>
  );
}

function SectionTitle({ icon, text }: { icon: React.ReactNode; text: string }) {
  return <div className="mb-2 flex items-center gap-1.5 text-xs uppercase tracking-wider text-neutral-500">{icon}{text}</div>;
}

function Badge({ color, children }: { color: string; children: React.ReactNode }) {
  return <span className={`inline-flex items-center gap-1 rounded-full px-2 py-0.5 text-xs font-medium ${color}`}>{children}</span>;
}

function AttrCard({ icon, label, value, sub }: { icon: React.ReactNode; label: string; value: string | number; sub: string }) {
  return (
    <div className="rounded-lg border border-neutral-700/50 bg-neutral-800/40 p-2.5">
      <div className="flex items-center gap-1.5">{icon}<span className="text-xs text-neutral-400">{label}</span></div>
      <div className="mt-1 text-sm font-semibold text-neutral-100">{value}</div>
      <div className="text-xs text-neutral-500">{sub}</div>
    </div>
  );
}

function VitalBar({ label, value, max, color, icon, inverse }: { label: string; value: number; max: number; color: string; icon: React.ReactNode; inverse?: boolean }) {
  const pct = Math.max(0, Math.min(100, (value / max) * 100));
  const display = inverse ? `${Math.round(value)} ticks left` : `${Math.round(value)} / ${Math.round(max)}`;
  return (
    <div>
      <div className="mb-1 flex items-center justify-between text-xs">
        <span className="flex items-center gap-1.5 text-neutral-400">{icon}{label}</span>
        <span className="font-mono text-neutral-300">{display}</span>
      </div>
      <div className="h-2 overflow-hidden rounded-full bg-neutral-700/60">
        <div className={`h-full rounded-full ${color} transition-all duration-300`} style={{ width: `${pct}%` }} />
      </div>
    </div>
  );
}
