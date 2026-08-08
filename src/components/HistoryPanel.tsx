import { useState } from 'react';
import { Save, FolderOpen, Trash2, Loader2 } from 'lucide-react';
import type { SnapshotRow } from '@/lib/supabase';
import { supabase, supabaseEnabled } from '@/lib/supabase';
import type { Simulation } from '@/sim/simulation';

type Props = {
  snapshots: SnapshotRow[];
  loading: boolean;
  sim: Simulation;
  onSaved: () => void;
  onLoad: (row: SnapshotRow) => void;
  onDeleted: () => void;
};

export function HistoryPanel({
  snapshots,
  loading,
  sim,
  onSaved,
  onLoad,
  onDeleted,
}: Props) {
  const [name, setName] = useState('');
  const [saving, setSaving] = useState(false);

  const handleSave = async () => {
    if (!supabase || sim.population === 0) return;
    setSaving(true);
    const snapshot = {
      name: name.trim() || `Experiment @ tick ${sim.tick}`,
      tick: sim.tick,
      population: sim.population,
      species_count: sim.speciesCount,
      generations: sim.generations,
      snapshot: serializeSim(sim),
    };
    const { error } = await supabase.from('simulation_snapshots').insert(snapshot);
    setSaving(false);
    if (!error) {
      setName('');
      onSaved();
    }
  };

  const handleDelete = async (id: string) => {
    if (!supabase) return;
    const { error } = await supabase.from('simulation_snapshots').delete().eq('id', id);
    if (!error) onDeleted();
  };

  return (
    <div className="space-y-3">
      <div className="flex gap-2">
        <input
          value={name}
          onChange={(e) => setName(e.target.value)}
          placeholder="Name this experiment..."
          className="flex-1 rounded-lg border border-neutral-700 bg-neutral-800 px-3 py-2 text-sm text-neutral-100 placeholder:text-neutral-500 focus:border-neutral-400 focus:outline-none"
        />
        <button
          onClick={handleSave}
          disabled={!supabaseEnabled || saving || sim.population === 0}
          title={!supabaseEnabled ? 'Cloud history is not configured (no Supabase project set)' : undefined}
          className="flex items-center gap-2 rounded-lg bg-neutral-200 px-3 py-2 text-sm font-semibold text-neutral-900 transition hover:bg-white disabled:opacity-50 active:scale-95"
        >
          {saving ? <Loader2 size={16} className="animate-spin" /> : <Save size={16} />}
          Save
        </button>
      </div>

      {!supabaseEnabled ? (
        <p className="text-sm text-neutral-500 italic">
          Cloud history is disabled — this build isn't connected to a Supabase project. The
          simulation itself runs fully offline; only save/load of past experiments is unavailable.
        </p>
      ) : loading ? (
        <div className="flex items-center justify-center py-6 text-neutral-500">
          <Loader2 className="animate-spin" size={20} />
        </div>
      ) : snapshots.length === 0 ? (
        <p className="text-sm text-neutral-500 italic">
          No saved experiments yet. Run a simulation and save it to compare evolutionary paths.
        </p>
      ) : (
        <div className="space-y-2 max-h-64 overflow-y-auto pr-1">
          {snapshots.map((s) => (
            <div
              key={s.id}
              className="flex items-center gap-2 rounded-lg border border-neutral-700/50 bg-neutral-800/40 p-2"
            >
              <button
                onClick={() => onLoad(s)}
                className="flex flex-1 items-center gap-2 text-left"
              >
                <FolderOpen size={16} className="text-neutral-300 shrink-0" />
                <div className="min-w-0">
                  <div className="truncate text-sm font-medium text-neutral-200">
                    {s.name}
                  </div>
                  <div className="text-xs text-neutral-500">
                    Pop {s.population} · {s.species_count} species · gen {s.generations} · tick {s.tick}
                  </div>
                </div>
              </button>
              <button
                onClick={() => handleDelete(s.id)}
                className="rounded-md p-1.5 text-neutral-500 transition hover:bg-red-950/50 hover:text-red-400"
              >
                <Trash2 size={15} />
              </button>
            </div>
          ))}
        </div>
      )}
    </div>
  );
}

function serializeSim(sim: Simulation) {
  return {
    tick: sim.tick,
    population: sim.population,
    speciesCount: sim.speciesCount,
    generations: sim.generations,
    organisms: sim.organisms.map((o) => ({
      id: o.id,
      x: o.x,
      y: o.y,
      angle: o.angle,
      energy: o.energy,
      age: o.age,
      generation: o.generation,
      genome: o.genome,
      speciesId: o.speciesId,
      colonyId: o.colonyId,
      colonyRole: o.colonyRole,
      threatLevel: o.threatLevel,
      buildCooldown: o.buildCooldown,
      carrying: o.carrying,
      knowledgeBoost: o.knowledgeBoost,
      biomeId: o.biomeId,
      brain: o.brain ? {
        nInputs: o.brain.nInputs,
        nHidden: o.brain.nHidden,
        nOutputs: o.brain.nOutputs,
        weights: Array.from(o.brain.weights),
      } : null,
      infected: o.infected,
      infectionTimer: o.infectionTimer,
      symbiosisPartner: o.symbiosisPartner,
      socialRank: o.socialRank,
      clusterId: o.clusterId,
      fitness: o.fitness,
      tumbleTimer: o.tumbleTimer,
      inBiofilm: o.inBiofilm,
      biofilmId: o.biofilmId,
      sporeMode: o.sporeMode,
      sporeTimer: o.sporeTimer,
    })),
    species: sim.species.map((s) => ({
      id: s.id,
      hue: s.hue,
      count: s.count,
      representative: s.representative,
      generations: s.generations,
      avgIntelligence: s.avgIntelligence,
      rank: s.rank,
      totalKills: s.totalKills,
      structuresBuilt: s.structuresBuilt,
      civilizationLevel: s.civilizationLevel,
      knowledgeDiscovered: s.knowledgeDiscovered,
      evolutionLeaps: s.evolutionLeaps,
    })),
    colonies: sim.colonies.map((c) => ({
      id: c.id,
      speciesId: c.speciesId,
      members: c.members,
      centerX: c.centerX,
      centerY: c.centerY,
      hue: c.hue,
      nestId: c.nestId,
    })),
    structures: sim.structures.map((s) => ({
      id: s.id,
      type: s.type,
      x: s.x,
      y: s.y,
      speciesId: s.speciesId,
      colonyId: s.colonyId,
      hp: s.hp,
      maxHp: s.maxHp,
      radius: s.radius,
      hue: s.hue,
      age: s.age,
      materials: s.materials,
      tier: s.tier,
      contributingColonies: s.contributingColonies,
    })),
    biomes: sim.biomes.map((b) => ({
      id: b.id,
      type: b.type,
      cx: b.cx,
      cy: b.cy,
      radius: b.radius,
      foodRate: b.foodRate,
      energyDrain: b.energyDrain,
      speedMod: b.speedMod,
      hue: b.hue,
    })),
    remains: sim.remains.map((r) => ({
      id: r.id,
      x: r.x,
      y: r.y,
      energy: r.energy,
      age: r.age,
      hue: r.hue,
    })),
    knowledgeNodes: sim.knowledgeNodes.map((kn) => ({
      id: kn.id,
      x: kn.x,
      y: kn.y,
      intelBoost: kn.intelBoost,
      discovered: kn.discovered,
      discoveredBy: kn.discoveredBy,
      age: kn.age,
    })),
    stats: sim.stats,
    settings: {
      ...sim.settings,
      boundaryMode: sim.settings.boundaryMode,
      blueprints: sim.settings.blueprints,
      neuralBrains: sim.settings.neuralBrains,
      generationCycleTicks: sim.settings.generationCycleTicks,
      worldExpansion: sim.settings.worldExpansion,
      worldExpansionInterval: sim.settings.worldExpansionInterval,
      autoCheckpoint: sim.settings.autoCheckpoint,
      chemicalField: sim.settings.chemicalField,
      microbialBehavior: sim.settings.microbialBehavior,
      endlessGeneration: sim.settings.endlessGeneration,
      diseaseEvents: sim.settings.diseaseEvents,
      advancedBiology: sim.settings.advancedBiology,
      socialBehavior: sim.settings.socialBehavior,
      neutralDrift: sim.settings.neutralDrift,
    },
  };
}
