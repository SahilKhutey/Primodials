import { useCallback, useEffect, useRef, useState } from 'react';
import {
  Dna, FlaskConical, Layers, Settings, TrendingUp, Microscope,
  Maximize, Monitor, LineChart,
} from 'lucide-react';
import { Simulation } from '@/sim/simulation';
import { DEFAULT_SETTINGS, type SimSettings, type Species, type Colony, type BoundaryMode, type GodTool } from '@/sim/types';
import { SimCanvas } from '@/components/SimCanvas';
import { ControlBar, type SimSpeed } from '@/components/ControlBar';
import { StatsPanel } from '@/components/StatsPanel';
import { SpeciesPanel } from '@/components/SpeciesPanel';
import { HistoryPanel } from '@/components/HistoryPanel';
import { InspectorPanel } from '@/components/InspectorPanel';
import { EvolutionChart } from '@/components/EvolutionChart';
import { AmbientHUD } from '@/components/AmbientHUD';
import { WallpaperDock } from '@/components/WallpaperDock';
import { CinematicCamera } from '@/sim/cinematicCamera';
import { supabase, supabaseEnabled, type SnapshotRow } from '@/lib/supabase';
import { THEMES, getTheme, DEFAULT_THEME_ID, PACING_PRESETS, type WallpaperTheme, type PacingPreset } from '@/sim/themes';
import { EcosystemDiary } from '@/sim/diary';
import { DiaryPanel } from '@/components/DiaryPanel';
import { AmbientOrganismCard } from '@/components/AmbientOrganismCard';
import { BookOpen } from 'lucide-react';

function App() {
  const simRef = useRef(new Simulation(Date.now()));
  const [, force] = useState(0);
  const [running, setRunning] = useState(true);
  const [speed, setSpeed] = useState<SimSpeed>(1);
  const [showSense, setShowSense] = useState(false);
  const [showColonies, setShowColonies] = useState(true);
  const [snapshots, setSnapshots] = useState<SnapshotRow[]>([]);
  const [loadingHistory, setLoadingHistory] = useState(true);
  const [activeTab, setActiveTab] = useState<'species' | 'inspector' | 'evolution' | 'history' | 'settings'>('species');
  const [selectedId, setSelectedId] = useState<number | null>(null);
  const [settings, setSettings] = useState<SimSettings>(DEFAULT_SETTINGS);
  const [wallpaperMode, setWallpaperMode] = useState(false);
  const [isFullscreen, setIsFullscreen] = useState(false);
  const [themeId, setThemeId] = useState(DEFAULT_THEME_ID);
  const [pacing, setPacing] = useState<PacingPreset>('peaceful');
  const [showDiary, setShowDiary] = useState(false);
  const [godTool, setGodTool] = useState<GodTool>('select');
  const theme = getTheme(themeId);
  const cinematicRef = useRef<CinematicCamera | null>(null);
  const diaryRef = useRef<EcosystemDiary | null>(null);
  const prevThemeIdRef = useRef<string>(themeId);

  if (!diaryRef.current) {
    diaryRef.current = new EcosystemDiary();
  }

  if (!cinematicRef.current) {
    cinematicRef.current = new CinematicCamera(simRef.current);
  }

  // Apply a theme+pacing combination's derived settings to the simulation.
  // IMPORTANT: this is only ever called from explicit user actions (picking a
  // theme or a pacing preset in the Wallpaper dock) — never as a passive side
  // effect of switching into Wallpaper Mode. Simply entering Wallpaper Mode
  // must not silently discard settings the user tuned in Simulation View.
  const applyThemeAndPacing = (nextThemeId: string, nextPacing: PacingPreset) => {
    const nextTheme = getTheme(nextThemeId);
    const pacingCfg = PACING_PRESETS[nextPacing];
    const sim = simRef.current;
    sim.settings = {
      ...sim.settings,
      maxPopulation: Math.round(nextTheme.maxPopulation * pacingCfg.maxPopulationMod),
      foodGrowthRate: Math.round(nextTheme.foodRate * pacingCfg.foodRateMod),
      mutationRate: 0.15 * pacingCfg.mutationMod,
    };
  };

  const handleThemeChange = (nextThemeId: string) => {
    setThemeId(nextThemeId);
    applyThemeAndPacing(nextThemeId, pacing);
  };

  const handlePacingChange = (nextPacing: PacingPreset) => {
    setPacing(nextPacing);
    applyThemeAndPacing(themeId, nextPacing);
  };

  // Re-render UI panels ~4x/sec to reflect sim state
  useEffect(() => {
    const id = setInterval(() => {
      force((n) => n + 1);
      // Update diary each tick
      if (running && diaryRef.current) {
        diaryRef.current.update(simRef.current);
      }
    }, 250);
    return () => clearInterval(id);
  }, [running]);

  // Track theme changes for diary
  useEffect(() => {
    if (prevThemeIdRef.current !== themeId && diaryRef.current) {
      diaryRef.current.notifyThemeChanged(
        THEMES.find((t) => t.id === prevThemeIdRef.current)?.displayName ?? prevThemeIdRef.current,
        THEMES.find((t) => t.id === themeId)?.displayName ?? themeId,
      );
      prevThemeIdRef.current = themeId;
    }
  }, [themeId]);

  // Spacebar to toggle pause/play
  useEffect(() => {
    const onKey = (e: KeyboardEvent) => {
      if (e.code === 'Space' && !(e.target instanceof HTMLInputElement) && !(e.target instanceof HTMLTextAreaElement)) {
        e.preventDefault();
        setRunning((r) => !r);
      }
    };
    window.addEventListener('keydown', onKey);
    return () => window.removeEventListener('keydown', onKey);
  }, []);

  const loadHistory = useCallback(async () => {
    if (!supabase) return;
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
    registerAutoCheckpoint(simRef.current);
  }, [loadHistory]);

  const registerAutoCheckpoint = (sim: Simulation) => {
    sim.onAutoCheckpoint = async (cycle: number, tick: number) => {
      if (!sim.settings.autoCheckpoint) return;
      if (!supabase) return;
      const snapshot = {
        name: `Auto-checkpoint · Cycle ${cycle}`,
        tick,
        population: sim.population,
        species_count: sim.species.length,
        generations: sim.stats.maxGeneration,
        snapshot: {
          tick: sim.tick,
          organisms: sim.organisms.map((o) => ({
            id: o.id, x: o.x, y: o.y, angle: o.angle,
            energy: o.energy, age: o.age, generation: o.generation,
            genome: o.genome, speciesId: o.speciesId,
            colonyId: o.colonyId, colonyRole: o.colonyRole,
            threatLevel: o.threatLevel, buildCooldown: o.buildCooldown,
            carrying: o.carrying, knowledgeBoost: o.knowledgeBoost,
            biomeId: o.biomeId,
            brain: o.brain ? {
              nInputs: o.brain.nInputs, nHidden: o.brain.nHidden,
              nOutputs: o.brain.nOutputs, weights: Array.from(o.brain.weights),
            } : null,
            fitness: o.fitness,
            tumbleTimer: o.tumbleTimer,
            inBiofilm: o.inBiofilm,
            biofilmId: o.biofilmId,
            sporeMode: o.sporeMode,
            sporeTimer: o.sporeTimer,
            infected: o.infected,
            infectionTimer: o.infectionTimer,
            symbiosisPartner: o.symbiosisPartner,
            socialRank: o.socialRank,
            clusterId: o.clusterId,
          })),
          food: sim.food,
          species: sim.species,
          colonies: sim.colonies,
          structures: sim.structures,
          biomes: sim.biomes,
          remains: sim.remains,
          knowledgeNodes: sim.knowledgeNodes,
          stats: sim.stats,
          settings: sim.settings,
        },
      };
      if (!supabaseEnabled) return;
      await supabase.from('simulation_snapshots').insert(snapshot);
      loadHistory();
    };
  };

  const handleReset = () => {
    const sim = new Simulation(Date.now(), settings);
    registerAutoCheckpoint(sim);
    simRef.current = sim;
    cinematicRef.current = new CinematicCamera(sim);
    setRunning(true);
  };

  const handleSeedLife = () => {
    simRef.current.seedLife(10);
  };

  const handleStep = () => {
    if (!running) simRef.current.step();
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
      brain: o.brain ? {
        nInputs: o.brain.nInputs,
        nHidden: o.brain.nHidden,
        nOutputs: o.brain.nOutputs,
        weights: new Float32Array(o.brain.weights),
      } : null,
      fitness: o.fitness ?? 0,
      lastInputs: null,
      lastOutputs: null,
      tumbleTimer: o.tumbleTimer ?? 0,
      inBiofilm: o.inBiofilm ?? false,
      biofilmId: o.biofilmId ?? null,
      sporeMode: o.sporeMode ?? false,
      sporeTimer: o.sporeTimer ?? 0,
      infected: o.infected ?? false,
      infectionTimer: o.infectionTimer ?? 0,
      symbiosisPartner: o.symbiosisPartner ?? null,
      socialRank: (o.socialRank as 'alpha' | 'beta' | 'omega' | 'solitary') ?? 'solitary',
      clusterId: o.clusterId ?? null,
      hibernating: o.hibernating ?? false,
      sonarPulse: o.sonarPulse ?? 0,
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
      type: b.type as 'plains' | 'forest' | 'desert' | 'tundra' | 'wetland' | 'ocean' | 'volcanic' | 'toxic' | 'crystal',
    }));
    sim.remains = snap.remains ?? [];
    sim.knowledgeNodes = (snap.knowledgeNodes ?? []).map((kn) => ({
      ...kn,
      discoveredBy: kn.discoveredBy ?? null,
    }));
    registerAutoCheckpoint(sim);
    simRef.current = sim;
    cinematicRef.current = new CinematicCamera(sim);
    setSettings(snap.settings);
    setRunning(true);
  };

  const applySettings = () => {
    simRef.current.settings = { ...settings };
  };

  const handleToggleSetting = (key: keyof SimSettings, value: boolean | BoundaryMode) => {
    const next = { ...settings, [key]: value };
    setSettings(next);
    simRef.current.settings = { ...next };
  };

  const toggleFullscreen = () => {
    if (!document.fullscreenElement) {
      document.documentElement.requestFullscreen().catch(() => {});
      setIsFullscreen(true);
    } else {
      document.exitFullscreen().catch(() => {});
      setIsFullscreen(false);
    }
  };

  useEffect(() => {
    const onFsChange = () => setIsFullscreen(!!document.fullscreenElement);
    document.addEventListener('fullscreenchange', onFsChange);
    return () => document.removeEventListener('fullscreenchange', onFsChange);
  }, []);

  const sim = simRef.current;

  // ── Wallpaper mode: full-screen immersive layout ──────────────────
  if (wallpaperMode) {
    return (
      <div className="fixed inset-0 overflow-hidden bg-black">
        <SimCanvas
          sim={sim}
          running={running}
          speed={speed}
          showSense={showSense}
          selectedId={selectedId}
          onSelect={(id) => { setSelectedId(id); if (id !== null) setActiveTab('inspector'); }}
          showColonies={showColonies}
          wallpaperMode
          cinematic={cinematicRef.current!}
          theme={theme}
        />
        <AmbientHUD sim={sim} visible />
        <WallpaperDock
          sim={sim}
          running={running}
          onToggleRun={() => setRunning((r) => !r)}
          onReset={handleReset}
          settings={settings}
          onToggleSetting={handleToggleSetting}
          cinematic={cinematicRef.current!}
          onToggleFullscreen={toggleFullscreen}
          isFullscreen={isFullscreen}
          themeId={themeId}
          onThemeChange={handleThemeChange}
          pacing={pacing}
          onPacingChange={handlePacingChange}
        />
        {/* Diary button — top left, below HUD */}
        <button
          onClick={() => setShowDiary(true)}
          className="fixed left-6 top-28 z-30 flex items-center gap-2 rounded-xl bg-neutral-950/60 px-3 py-2 text-xs font-semibold text-neutral-400 ring-1 ring-white/10 backdrop-blur-md transition hover:bg-neutral-900/80 hover:text-neutral-200"
        >
          <BookOpen size={15} />
          Diary
          {diaryRef.current && diaryRef.current.entries.length > 0 && (
            <span className="rounded-full bg-amber-500/20 px-1.5 py-0.5 text-[9px] font-bold text-amber-300">
              {diaryRef.current.entries.length}
            </span>
          )}
        </button>

        {/* Exit wallpaper mode button — top right, subtle */}
        <button
          onClick={() => setWallpaperMode(false)}
          className="fixed right-6 top-6 z-30 flex items-center gap-2 rounded-xl bg-neutral-950/60 px-3 py-2 text-xs font-semibold text-neutral-400 ring-1 ring-white/10 backdrop-blur-md transition hover:bg-neutral-900/80 hover:text-neutral-200"
        >
          <Monitor size={15} />
          Exit Wallpaper
        </button>

        {/* Diary panel modal */}
        <DiaryPanel
          diary={diaryRef.current!}
          visible={showDiary}
          onClose={() => setShowDiary(false)}
        />

        {/* Ambient Organism Inspector Popover for Wallpaper Mode */}
        {selectedId !== null && (() => {
          const org = sim.organisms.find((o) => o.id === selectedId && o.alive);
          return org ? (
            <AmbientOrganismCard org={org} onClose={() => setSelectedId(null)} />
          ) : null;
        })()}
      </div>
    );
  }

  // ── Sandbox mode: original full-featured layout ─────────────────────
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
          <div className="flex items-center gap-3">
            <div className="hidden items-center gap-2 rounded-full border border-neutral-800 bg-neutral-900/60 px-3.5 py-1.5 text-sm text-neutral-400 sm:flex">
              <span className="relative flex h-2 w-2">
                <span className={`absolute inline-flex h-full w-full rounded-full opacity-60 ${running ? 'animate-ping bg-emerald-400' : 'bg-neutral-500'}`} />
                <span className={`relative inline-flex h-2 w-2 rounded-full ${running ? 'bg-emerald-400' : 'bg-neutral-500'}`} />
              </span>
              <FlaskConical size={14} className="text-neutral-500" />
              <span className="font-medium text-neutral-300">{sim.population}</span>
              <span className="text-neutral-500">organisms alive</span>
            </div>
            {/* Wallpaper mode toggle */}
            <button
              onClick={() => setWallpaperMode(true)}
              className="flex items-center gap-2 rounded-xl bg-gradient-to-br from-cyan-500/20 to-violet-500/20 px-3.5 py-2 text-sm font-semibold text-cyan-200 ring-1 ring-cyan-400/20 transition hover:from-cyan-500/30 hover:to-violet-500/30 active:scale-95"
            >
              <Maximize size={16} />
              Wallpaper Mode
            </button>
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
              onStep={handleStep}
              speed={speed}
              onSpeedChange={setSpeed}
              showSense={showSense}
              onToggleSense={() => setShowSense((s) => !s)}
              onSeedLife={handleSeedLife}
              showColonies={showColonies}
              onToggleColonies={() => setShowColonies((s) => !s)}
              settings={settings}
              onToggleSetting={handleToggleSetting}
              activeGodTool={godTool}
              onSelectGodTool={setGodTool}
            />
            <div className="relative min-h-[680px] h-[680px] w-full overflow-hidden rounded-2xl bg-neutral-950 ring-1 ring-neutral-800/80 shadow-2xl shadow-black/80">
              <SimCanvas
                sim={sim}
                running={running}
                speed={speed}
                showSense={showSense}
                selectedId={selectedId}
                onSelect={(id) => { setSelectedId(id); if (id !== null) setActiveTab('inspector'); }}
                showColonies={showColonies}
                godTool={godTool}
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
                <TabButton active={activeTab === 'evolution'} onClick={() => setActiveTab('evolution')} icon={<LineChart size={15} />} label="Evolution" />
                <TabButton active={activeTab === 'history'} onClick={() => setActiveTab('history')} icon={<TrendingUp size={15} />} label="Experiments" />
                <TabButton active={activeTab === 'settings'} onClick={() => setActiveTab('settings')} icon={<Settings size={15} />} label="Settings" />
              </div>

              {activeTab === 'species' && <SpeciesPanel species={sim.species} colonies={sim.colonies} structures={sim.structures} />}
              {activeTab === 'inspector' && (
                <InspectorPanel sim={sim} selectedId={selectedId} onClear={() => setSelectedId(null)} />
              )}
              {activeTab === 'evolution' && (
                <EvolutionChart sim={sim} />
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
        <ToggleRow label="Microbial behavior" checked={settings.microbialBehavior} onChange={(v) => update('microbialBehavior', v)} />
        <ToggleRow label="Disease & immunity" checked={settings.diseaseEvents} onChange={(v) => update('diseaseEvents', v)} />
        <ToggleRow label="Advanced biology" checked={settings.advancedBiology} onChange={(v) => update('advancedBiology', v)} />
        <ToggleRow label="Social hierarchy" checked={settings.socialBehavior} onChange={(v) => update('socialBehavior', v)} />
        <ToggleRow label="Neutral genetic drift" checked={settings.neutralDrift} onChange={(v) => update('neutralDrift', v)} />
        <ToggleRow label="Endless generation cycles" checked={settings.endlessGeneration} onChange={(v) => update('endlessGeneration', v)} />
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
