import { useCallback, useEffect, useRef, useState } from 'react';
import { Dna, FlaskConical, Layers, Settings, TrendingUp, Microscope } from 'lucide-react';
import { Simulation } from '@/sim/simulation';
import { DEFAULT_SETTINGS, type SimSettings, type Species, type Colony, type BoundaryMode } from '@/sim/types';
import { SimCanvas } from '@/components/SimCanvas';
import { ControlBar } from '@/components/ControlBar';
import { StatsPanel } from '@/components/StatsPanel';
import { SpeciesPanel } from '@/components/SpeciesPanel';
import { HistoryPanel } from '@/components/HistoryPanel';
import { InspectorPanel } from '@/components/InspectorPanel';
import { supabase, type SnapshotRow } from '@/lib/supabase';

function App() {
  const simRef = useRef(new Simulation(Date.now()));
  const [, force] = useState(0);
  const [running, setRunning] = useState(true);
  const [showSense, setShowSense] = useState(false);
  const [showColonies, setShowColonies] = useState(true);
  const [snapshots, setSnapshots] = useState<SnapshotRow[]>([]);
  const [loadingHistory, setLoadingHistory] = useState(true);
  const [activeTab, setActiveTab] = useState<'species' | 'inspector' | 'history' | 'settings'>('species');
  const [selectedId, setSelectedId] = useState<number | null>(null);
  const [settings, setSettings] = useState<SimSettings>(DEFAULT_SETTINGS);

  // Re-render UI panels ~4x/sec to reflect sim state
  useEffect(() => {
    const id = setInterval(() => force((n) => n + 1), 250);
    return () => clearInterval(id);
  }, []);

  const loadHistory = useCallback(async () => {
    setLoadingHistory(true);
    const { data, error } = await supabase
      .from('simulation_snapshots')
      .select('*')
      .order('created_at', { ascending: false })
      .limit(50);
    if (!error && data) setSnapshots(data as SnapshotRow[]);
    setLoadingHistory(false);
  }, []);

  useEffect(() => {
    loadHistory();
  }, [loadHistory]);

  const handleReset = () => {
    simRef.current = new Simulation(Date.now(), settings);
    setRunning(true);
  };

  const handleSeedLife = () => {
    simRef.current.seedLife(10);
  };

  const handleLoad = (row: SnapshotRow) => {
    const snap = row.snapshot;
    const sim = new Simulation(Date.now(), snap.settings);
    sim.tick = snap.tick;
    sim.stats = snap.stats;
    sim.organisms = snap.organisms.map((o) => ({
      ...o,
      vx: 0,
      vy: 0,
      alive: true,
      reproductionCooldown: 30,
      threatLevel: o.threatLevel ?? 0,
      buildCooldown: o.buildCooldown ?? 0,
      carrying: o.carrying ?? 0,
      knowledgeBoost: o.knowledgeBoost ?? 0,
      biomeId: o.biomeId ?? 0,
    }));
    sim.species = snap.species.map((s: Species & { generations: number; avgIntelligence: number; rank: number; totalKills: number; structuresBuilt: number; civilizationLevel: number; knowledgeDiscovered: number; evolutionLeaps: number }) => ({
      ...s,
      avgIntelligence: s.avgIntelligence ?? 0,
      rank: s.rank ?? 0,
      totalKills: s.totalKills ?? 0,
      structuresBuilt: s.structuresBuilt ?? 0,
      civilizationLevel: s.civilizationLevel ?? 0,
      knowledgeDiscovered: s.knowledgeDiscovered ?? 0,
      evolutionLeaps: s.evolutionLeaps ?? 0,
    }));
    sim.colonies = (snap.colonies ?? []).map((c) => ({ ...c, nestId: c.nestId ?? null }));
    sim.structures = (snap.structures ?? []).map((s) => ({
      ...s,
      type: s.type as 'nest' | 'cache' | 'watchtower' | 'wall' | 'monument' | 'sanctuary' | 'observatory' | 'bridge',
      materials: s.materials ?? 0,
      tier: (s.tier ?? 'basic') as 'basic' | 'advanced',
      contributingColonies: s.contributingColonies ?? [],
    }));
    sim.biomes = (snap.biomes ?? []).map((b) => ({
      ...b,
      type: b.type as 'plains' | 'forest' | 'desert' | 'tundra' | 'wetland',
    }));
    sim.remains = snap.remains ?? [];
    sim.knowledgeNodes = (snap.knowledgeNodes ?? []).map((kn) => ({
      ...kn,
      discoveredBy: kn.discoveredBy ?? null,
    }));
    simRef.current = sim;
    setSettings(snap.settings);
    setRunning(true);
  };

  const applySettings = () => {
    const handleToggleSetting = (key: keyof SimSettings, value: boolean | BoundaryMode) => {
    const next = { ...settings, [key]: value };
    setSettings(next);
    simRef.current.settings = { ...next };
  };

  const sim = simRef.current;
    sim.settings = { ...settings };
  };

  const handleToggleSetting = (key: keyof SimSettings, value: boolean | BoundaryMode) => {
    const next = { ...settings, [key]: value };
    setSettings(next);
    simRef.current.settings = { ...next };
  };

  const sim = simRef.current;

  return (
    <div className="min-h-screen bg-neutral-950 text-neutral-100 app-bg">
      {/* Header */}
      <header className="sticky top-0 z-20 border-b border-neutral-800/80 bg-neutral-950/70 backdrop-blur-xl">
        <div className="mx-auto flex max-w-[1600px] items-center justify-between px-6 py-4">
          <div className="flex items-center gap-3">
            <div className="relative">
              <div className="absolute inset-0 rounded-xl bg-gradient-to-br from-cyan-500/30 to-violet-500/20 blur-md" />
              <div className="relative rounded-xl bg-gradient-to-br from-neutral-700 to-neutral-800 p-2.5 shadow-lg shadow-black/40 ring-1 ring-white/10">
                <Dna className="text-cyan-300" size={26} />
              </div>
            </div>
            <div>
              <h1 className="text-xl font-bold tracking-tight bg-gradient-to-r from-neutral-100 to-neutral-300 bg-clip-text text-transparent">Polygonal Primordials</h1>
              <p className="text-xs text-neutral-500">Artificial life &amp; evolution sandbox</p>
            </div>
          </div>
          <div className="hidden items-center gap-2 rounded-full border border-neutral-800 bg-neutral-900/60 px-3.5 py-1.5 text-sm text-neutral-400 sm:flex">
            <span className="relative flex h-2 w-2">
              <span className={`absolute inline-flex h-full w-full rounded-full opacity-60 ${running ? 'animate-ping bg-emerald-400' : 'bg-neutral-500'}`} />
              <span className={`relative inline-flex h-2 w-2 rounded-full ${running ? 'bg-emerald-400' : 'bg-neutral-500'}`} />
            </span>
            <FlaskConical size={14} className="text-neutral-500" />
            <span className="font-medium text-neutral-300">{sim.population}</span>
            <span className="text-neutral-500">organisms alive</span>
          </div>
        </div>
      </header>

      {/* Main */}
      <main className="mx-auto max-w-[1600px] px-6 py-6">
        <div className="grid grid-cols-1 gap-6 lg:grid-cols-[1fr_380px]">
          {/* Simulation */}
          <div className="flex flex-col gap-4">
            <ControlBar
              running={running}
              onToggleRun={() => setRunning((r) => !r)}
              onReset={handleReset}
              showSense={showSense}
              onToggleSense={() => setShowSense((s) => !s)}
              onSeedLife={handleSeedLife}
              showColonies={showColonies}
              onToggleColonies={() => setShowColonies((s) => !s)}
              settings={settings}
              onToggleSetting={handleToggleSetting}
            />
            <div className="relative aspect-[3/2] w-full overflow-hidden rounded-2xl bg-neutral-900 ring-1 ring-neutral-800/60">
              <SimCanvas
                sim={sim}
                running={running}
                showSense={showSense}
                selectedId={selectedId}
                onSelect={(id) => { setSelectedId(id); if (id !== null) setActiveTab('inspector'); }}
                showColonies={showColonies}
              />
            </div>
            <StatsPanel sim={sim} />
          </div>

          {/* Side panel */}
          <aside className="flex flex-col gap-4">
            <div className="rounded-2xl border border-neutral-800/80 bg-neutral-900/60 p-4 backdrop-blur-sm shadow-xl shadow-black/30">
              <div className="mb-3 flex gap-1 rounded-lg bg-neutral-800/60 p-1">
                <TabButton active={activeTab === 'species'} onClick={() => setActiveTab('species')} icon={<Layers size={15} />} label="Species" />
                <TabButton active={activeTab === 'inspector'} onClick={() => setActiveTab('inspector')} icon={<Microscope size={15} />} label="Inspector" />
                <TabButton active={activeTab === 'history'} onClick={() => setActiveTab('history')} icon={<TrendingUp size={15} />} label="Experiments" />
                <TabButton active={activeTab === 'settings'} onClick={() => setActiveTab('settings')} icon={<Settings size={15} />} label="Settings" />
              </div>

              {activeTab === 'species' && <SpeciesPanel species={sim.species} colonies={sim.colonies} structures={sim.structures} />}
              {activeTab === 'inspector' && (
                <InspectorPanel sim={sim} selectedId={selectedId} onClear={() => setSelectedId(null)} />
              )}
              {activeTab === 'history' && (
                <HistoryPanel
                  snapshots={snapshots}
                  loading={loadingHistory}
                  sim={sim}
                  onSaved={loadHistory}
                  onLoad={handleLoad}
                  onDeleted={loadHistory}
                />
              )}
              {activeTab === 'settings' && (
                <SettingsPanel settings={settings} onChange={setSettings} onApply={applySettings} />
              )}
            </div>
          </aside>
        </div>
      </main>
    </div>
  );
}

function TabButton({
  active,
  onClick,
  icon,
  label,
}: {
  active: boolean;
  onClick: () => void;
  icon: React.ReactNode;
  label: string;
}) {
  return (
    <button
      onClick={onClick}
      className={`flex flex-1 items-center justify-center gap-1.5 rounded-md px-2 py-1.5 text-xs font-semibold transition ${
        active ? 'bg-neutral-200 text-neutral-900 shadow' : 'text-neutral-400 hover:text-neutral-200'
      }`}
    >
      {icon}
      {label}
    </button>
  );
}

function SettingsPanel({
  settings,
  onChange,
  onApply,
}: {
  settings: SimSettings;
  onChange: (s: SimSettings) => void;
  onApply: () => void;
}) {
  const update = (key: keyof SimSettings, value: number | boolean | 'wrap' | 'reflect' | 'open') =>
    onChange({ ...settings, [key]: value });

  return (
    <div className="space-y-4">
      <Slider label="Initial population" value={settings.initialPopulation} min={5} max={100} step={5} onChange={(v) => update('initialPopulation', v)} />
      <Slider label="Initial food" value={settings.initialFood} min={20} max={300} step={10} onChange={(v) => update('initialFood', v)} />
      <Slider label="Food growth rate" value={settings.foodGrowthRate} min={0} max={10} step={1} onChange={(v) => update('foodGrowthRate', v)} />
      <Slider label="Mutation rate" value={settings.mutationRate} min={0} max={0.5} step={0.01} onChange={(v) => update('mutationRate', v)} />
      <Slider label="Reproduction threshold" value={settings.reproductionThreshold} min={80} max={250} step={10} onChange={(v) => update('reproductionThreshold', v)} />
      <Slider label="Max population" value={settings.maxPopulation} min={50} max={800} step={50} onChange={(v) => update('maxPopulation', v)} />
      <Slider label="Max food" value={settings.maxFood} min={50} max={500} step={25} onChange={(v) => update('maxFood', v)} />
      <Slider label="Colony radius" value={settings.colonyRadius} min={40} max={160} step={10} onChange={(v) => update('colonyRadius', v)} />

      <div className="space-y-2 border-t border-neutral-700/40 pt-3">
        <ToggleRow label="Sexual reproduction" checked={settings.sexualReproduction} onChange={(v) => update('sexualReproduction', v)} />
        <ToggleRow label="Colony formation" checked={settings.colonyFormation} onChange={(v) => update('colonyFormation', v)} />
        <ToggleRow label="Structure building" checked={settings.structureBuilding} onChange={(v) => update('structureBuilding', v)} />
        <ToggleRow label="Biomes" checked={settings.biomes} onChange={(v) => update('biomes', v)} />
        <ToggleRow label="Knowledge nodes" checked={settings.knowledgeNodes} onChange={(v) => update('knowledgeNodes', v)} />
        <ToggleRow label="Advanced blueprints" checked={settings.blueprints} onChange={(v) => update('blueprints', v)} />
        <div className="pt-1">
          <label className="text-xs font-semibold uppercase tracking-wider text-neutral-400">World Boundary</label>
          <div className="mt-2 flex gap-1 rounded-lg bg-neutral-800/60 p-1">
            {(['wrap', 'reflect', 'open'] as const).map((m) => (
              <button
                key={m}
                onClick={() => update('boundaryMode', m)}
                className={`flex-1 rounded-md px-3 py-1.5 text-xs font-semibold capitalize transition ${
                  settings.boundaryMode === m
                    ? 'bg-cyan-500/20 text-cyan-300 ring-1 ring-white/20'
                    : 'text-neutral-400 hover:text-neutral-200'
                }`}
              >
                {m === 'wrap' ? 'Wrap (endless)' : m === 'reflect' ? 'Reflect' : 'Open (walled)'}
              </button>
            ))}
          </div>
          <p className="mt-1.5 text-xs text-neutral-500">
            {settings.boundaryMode === 'wrap' && 'Creatures exiting one side reappear on the opposite side — the world is endless.'}
            {settings.boundaryMode === 'reflect' && 'Creatures bounce off the world edges like walls.'}
            {settings.boundaryMode === 'open' && 'The world has hard walls. Creatures are confined to the defined area.'}
          </p>
        </div>
      </div>

      <p className="text-xs text-neutral-500">
        Changes apply to the running simulation. Reset to start a new world with these settings.
      </p>
      <button
        onClick={onApply}
        className="w-full rounded-lg bg-neutral-200 px-4 py-2 text-sm font-semibold text-neutral-900 transition hover:bg-white active:scale-95"
      >
        Apply to running sim
      </button>
    </div>
  );
}

function ToggleRow({ label, checked, onChange }: { label: string; checked: boolean; onChange: (v: boolean) => void }) {
  return (
    <div className="flex items-center justify-between">
      <span className="text-sm text-neutral-300">{label}</span>
      <button
        onClick={() => onChange(!checked)}
        className={`relative h-6 w-11 rounded-full transition ${checked ? 'bg-neutral-200' : 'bg-neutral-700'}`}
      >
        <span
          className={`absolute top-0.5 h-5 w-5 rounded-full transition ${checked ? 'left-[22px] bg-neutral-900' : 'left-0.5 bg-neutral-400'}`}
        />
      </button>
    </div>
  );
}

function Slider({
  label,
  value,
  min,
  max,
  step,
  onChange,
}: {
  label: string;
  value: number;
  min: number;
  max: number;
  step: number;
  onChange: (v: number) => void;
}) {
  return (
    <div>
      <div className="mb-1 flex justify-between text-xs">
        <span className="text-neutral-400">{label}</span>
        <span className="font-mono text-neutral-200">{value}</span>
      </div>
      <input
        type="range"
        min={min}
        max={max}
        step={step}
        value={value}
        onChange={(e) => onChange(parseFloat(e.target.value))}
        className="w-full accent-neutral-200"
      />
    </div>
  );
}

export default App;
