// WallpaperRoot.tsx
// Zero-Supabase wallpaper render path.
// This module is only loaded when the app boots with ?wallpaper=1 or
// localStorage.wallpaperMode === 'true'. It NEVER imports from @/lib/supabase,
// so the wallpaper can boot on a fresh clone with no .env file at all.

import { useEffect, useRef, useState } from 'react';
import { Simulation } from '@/sim/simulation';
import { DEFAULT_SETTINGS, type SimSettings, type BoundaryMode } from '@/sim/types';
import { SimCanvas } from '@/components/SimCanvas';
import { AmbientHUD } from '@/components/AmbientHUD';
import { WallpaperDock } from '@/components/WallpaperDock';
import { CinematicCamera } from '@/sim/cinematicCamera';
import { EcosystemDiary } from '@/sim/diary';
import { DiaryPanel } from '@/components/DiaryPanel';
import { THEMES, getTheme, DEFAULT_THEME_ID, PACING_PRESETS, type PacingPreset } from '@/sim/themes';
import { AmbientOrganismCard } from '@/components/AmbientOrganismCard';
import { BookOpen, Monitor } from 'lucide-react';
import { useWallpaperSettings } from '@/hooks/useWallpaperSettings';

export function WallpaperRoot() {
  const simRef = useRef(new Simulation(Date.now()));
  const [, force] = useState(0);
  const [running, setRunning] = useState(true);
  const [isFullscreen, setIsFullscreen] = useState(false);
  const [showDiary, setShowDiary] = useState(false);
  const [selectedId, setSelectedId] = useState<number | null>(null);
  const [themeId, setThemeId] = useState(DEFAULT_THEME_ID);
  const [pacing, setPacing] = useState<PacingPreset>('peaceful');
  const [settings, setSettings] = useState<SimSettings>(DEFAULT_SETTINGS);

  const cinematicRef = useRef<CinematicCamera | null>(null);
  const diaryRef = useRef<EcosystemDiary | null>(null);
  const prevThemeIdRef = useRef<string>(themeId);

  const { wallpaperSettings } = useWallpaperSettings();

  if (!cinematicRef.current) cinematicRef.current = new CinematicCamera(simRef.current);
  if (!diaryRef.current) diaryRef.current = new EcosystemDiary();

  // Apply pacing + theme settings to sim
  useEffect(() => {
    const pacingCfg = PACING_PRESETS[pacing];
    const theme = getTheme(themeId);
    const sim = simRef.current;
    sim.settings = {
      ...sim.settings,
      maxPopulation: Math.round(
        Math.min(wallpaperSettings.maxPopulation, theme.maxPopulation) * pacingCfg.maxPopulationMod,
      ),
      foodGrowthRate: Math.round(theme.foodRate * pacingCfg.foodRateMod),
      mutationRate: wallpaperSettings.mutationRate * pacingCfg.mutationMod,
    };
  }, [themeId, pacing, wallpaperSettings]);

  // Performance: skip frames when quality is 'low'
  const simStepsPerTick = wallpaperSettings.quality === 'low' ? 1 : wallpaperSettings.quality === 'medium' ? 2 : 3;

  // Re-render UI ~4×/sec
  useEffect(() => {
    const id = setInterval(() => {
      force((n) => n + 1);
      if (running && diaryRef.current) diaryRef.current.update(simRef.current);
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

  // Auto-pause when a fullscreen app is detected (Electron only — browser ignores)
  useEffect(() => {
    if (!wallpaperSettings.autoPause) return;
    const onVisibility = () => {
      if (document.hidden) setRunning(false);
      else setRunning(true);
    };
    document.addEventListener('visibilitychange', onVisibility);
    return () => document.removeEventListener('visibilitychange', onVisibility);
  }, [wallpaperSettings.autoPause]);

  // Fullscreen sync
  useEffect(() => {
    const onFsChange = () => setIsFullscreen(!!document.fullscreenElement);
    document.addEventListener('fullscreenchange', onFsChange);
    return () => document.removeEventListener('fullscreenchange', onFsChange);
  }, []);

  const theme = getTheme(themeId);

  const handleReset = () => {
    const sim = new Simulation(Date.now(), settings);
    simRef.current = sim;
    cinematicRef.current = new CinematicCamera(sim);
    setRunning(true);
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

  const exitWallpaperMode = () => {
    localStorage.removeItem('wallpaperMode');
    window.location.href = '/';
  };

  return (
    <div className="fixed inset-0 overflow-hidden bg-black">
      <SimCanvas
        sim={simRef.current}
        running={running}
        speed={simStepsPerTick as 1 | 2 | 4 | 8}
        showSense={false}
        selectedId={selectedId}
        onSelect={(id) => setSelectedId(id)}
        showColonies
        wallpaperMode
        cinematic={cinematicRef.current!}
        theme={theme}
      />

      <AmbientHUD sim={simRef.current} visible />

      <WallpaperDock
        sim={simRef.current}
        running={running}
        onToggleRun={() => setRunning((r) => !r)}
        onReset={handleReset}
        settings={settings}
        onToggleSetting={handleToggleSetting}
        cinematic={cinematicRef.current!}
        onToggleFullscreen={toggleFullscreen}
        isFullscreen={isFullscreen}
        themeId={themeId}
        onThemeChange={setThemeId}
        pacing={pacing}
        onPacingChange={setPacing}
      />

      {/* Diary button — top left */}
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

      {/* Exit to sandbox — top right */}
      <button
        onClick={exitWallpaperMode}
        className="fixed right-6 top-6 z-30 flex items-center gap-2 rounded-xl bg-neutral-950/60 px-3 py-2 text-xs font-semibold text-neutral-400 ring-1 ring-white/10 backdrop-blur-md transition hover:bg-neutral-900/80 hover:text-neutral-200"
      >
        <Monitor size={15} />
        Exit Wallpaper
      </button>

      <DiaryPanel
        diary={diaryRef.current!}
        visible={showDiary}
        onClose={() => setShowDiary(false)}
      />

      {/* Ambient Organism Inspector Popover for Wallpaper Engine Mode */}
      {selectedId !== null && (() => {
        const org = simRef.current.organisms.find((o) => o.id === selectedId && o.alive);
        return org ? (
          <AmbientOrganismCard org={org} onClose={() => setSelectedId(null)} />
        ) : null;
      })()}
    </div>
  );
}
