import { useState } from 'react';
import {
  Play, Pause, RotateCcw, FlaskConical, Beaker,
  Brain, Globe, Save, Maximize, Minimize, Camera, Palette, Gauge, Settings, SlidersHorizontal, ChevronUp, ChevronDown, Share2,
} from 'lucide-react';
import type { Simulation } from '@/sim/simulation';
import type { SimSettings } from '@/sim/types';
import type { CinematicCamera } from '@/sim/cinematicCamera';
import { THEMES, PACING_PRESETS, type PacingPreset } from '@/sim/themes';
import { useWallpaperSettings, type QualityPreset } from '@/hooks/useWallpaperSettings';
import { applyQualityProfile } from '@/lib/qualityProfiles';

type Props = {
  sim: Simulation;
  running: boolean;
  onToggleRun: () => void;
  onReset: () => void;
  settings: SimSettings;
  onToggleSetting: (key: keyof SimSettings, value: boolean) => void;
  cinematic: CinematicCamera;
  onToggleFullscreen: () => void;
  isFullscreen: boolean;
  themeId: string;
  onThemeChange: (id: string) => void;
  pacing: PacingPreset;
  onPacingChange: (p: PacingPreset) => void;
  onOpenShare?: () => void;
};

// Floating dock for wallpaper mode with persistent controls toggle
export function WallpaperDock({
  sim, running, onToggleRun, onReset, settings, onToggleSetting,
  cinematic, onToggleFullscreen, isFullscreen,
  themeId, onThemeChange, pacing, onPacingChange, onOpenShare,
}: Props) {
  const [visible, setVisible] = useState(true);
  const [showThemes, setShowThemes] = useState(false);
  const [showPacing, setShowPacing] = useState(false);
  const [showSettings, setShowSettings] = useState(false);

  const { wallpaperSettings, updateSetting } = useWallpaperSettings();

  return (
    <div className="fixed bottom-3 left-1/2 z-30 -translate-x-1/2 flex flex-col items-center">
      {/* Floating Toggle Button (Always Visible) */}
      {!visible && (
        <button
          onClick={() => setVisible(true)}
          className="flex items-center gap-2 rounded-full border border-white/20 bg-neutral-950/80 px-4 py-2 text-xs font-semibold text-neutral-200 shadow-2xl backdrop-blur-xl transition hover:bg-neutral-900 active:scale-95"
        >
          <SlidersHorizontal size={14} className="text-cyan-400" />
          <span>Wallpaper Controls</span>
          <ChevronUp size={14} className="text-neutral-400" />
        </button>
      )}

      {/* Main Control Panel Dock */}
      {visible && (
        <div className="mx-auto flex flex-col items-center gap-2 transition-all duration-300 pb-2">
          {/* Hide button header */}
          <button
            onClick={() => setVisible(false)}
            className="flex items-center gap-1 rounded-full bg-neutral-950/60 px-3 py-0.5 text-[10px] font-medium text-neutral-400 ring-1 ring-white/10 hover:text-neutral-200"
          >
            <span>Hide Panel</span>
            <ChevronDown size={12} />
          </button>
        {/* Main dock */}
        <div className="flex items-center gap-1.5 rounded-2xl border border-white/10 bg-neutral-950/70 px-3 py-2 shadow-2xl shadow-black/60 backdrop-blur-xl">
          {/* Run / Pause */}
          <DockButton
            onClick={onToggleRun}
            active={running}
            icon={running ? <Pause size={16} /> : <Play size={16} />}
            label={running ? 'Pause' : 'Play'}
            activeClass="bg-emerald-500/20 text-emerald-300"
          />

          {/* Reset */}
          <DockButton
            onClick={onReset}
            icon={<RotateCcw size={16} />}
            label="Reset"
          />

          <Divider />

          {/* Camera mode */}
          <DockButton
            onClick={() => cinematic.setMode('auto')}
            active={cinematic.mode === 'auto'}
            icon={<Camera size={16} />}
            label="Auto"
            activeClass="bg-cyan-500/20 text-cyan-300"
          />

          {/* Fullscreen */}
          <DockButton
            onClick={onToggleFullscreen}
            active={isFullscreen}
            icon={isFullscreen ? <Minimize size={16} /> : <Maximize size={16} />}
            label="Fullscreen"
            activeClass="bg-blue-500/20 text-blue-300"
          />

          <Divider />

          {/* Theme picker */}
          <DockButton
            onClick={() => { setShowThemes((s) => !s); setShowPacing(false); setShowSettings(false); }}
            active={showThemes}
            icon={<Palette size={16} />}
            label="Theme"
            activeClass="bg-amber-500/20 text-amber-300"
          />

          {/* Pacing */}
          <DockButton
            onClick={() => { setShowPacing((s) => !s); setShowThemes(false); setShowSettings(false); }}
            active={showPacing}
            icon={<Gauge size={16} />}
            label="Pacing"
            activeClass="bg-teal-500/20 text-teal-300"
          />

          {/* Wallpaper Settings */}
          <DockButton
            onClick={() => { setShowSettings((s) => !s); setShowThemes(false); setShowPacing(false); }}
            active={showSettings}
            icon={<Settings size={16} />}
            label="Settings"
            activeClass="bg-purple-500/20 text-purple-300"
          />

          {onOpenShare && (
            <DockButton
              onClick={onOpenShare}
              icon={<Share2 size={16} />}
              label="Share"
            />
          )}

          <Divider />

          {/* Feature toggles */}
          <DockToggle
            active={settings.chemicalField}
            onClick={() => onToggleSetting('chemicalField', !settings.chemicalField)}
            icon={<FlaskConical size={15} />}
            label="Chem"
            activeClass="bg-lime-500/20 text-lime-300"
          />
          <DockToggle
            active={settings.microbialBehavior}
            onClick={() => onToggleSetting('microbialBehavior', !settings.microbialBehavior)}
            icon={<Beaker size={15} />}
            label="Micro"
            activeClass="bg-green-500/20 text-green-300"
          />
          <DockToggle
            active={settings.neuralBrains}
            onClick={() => onToggleSetting('neuralBrains', !settings.neuralBrains)}
            icon={<Brain size={15} />}
            label="Brain"
            activeClass="bg-violet-500/20 text-violet-300"
          />
          <DockToggle
            active={settings.worldExpansion}
            onClick={() => onToggleSetting('worldExpansion', !settings.worldExpansion)}
            icon={<Globe size={15} />}
            label="World"
            activeClass="bg-blue-500/20 text-blue-300"
          />
          <DockToggle
            active={settings.autoCheckpoint}
            onClick={() => onToggleSetting('autoCheckpoint', !settings.autoCheckpoint)}
            icon={<Save size={15} />}
            label="Save"
            activeClass="bg-indigo-500/20 text-indigo-300"
          />
        </div>

        {/* Population indicator bar */}
        <div className="flex items-center gap-2 rounded-full bg-neutral-950/60 px-3 py-1 ring-1 ring-white/5 backdrop-blur-md">
          <div className="relative flex h-1.5 w-1.5">
            <span className={`absolute inline-flex h-full w-full animate-ping rounded-full opacity-75 ${running ? 'bg-emerald-400' : 'bg-neutral-500'}`} />
            <span className={`relative inline-flex h-1.5 w-1.5 rounded-full ${running ? 'bg-emerald-400' : 'bg-neutral-500'}`} />
          </div>
          <span className="text-[10px] font-medium text-neutral-400">
            {sim.population} organisms · {sim.species.length} species · gen {sim.stats.maxGeneration}
          </span>
        </div>

        {/* Theme picker panel */}
        {showThemes && (
          <div className="mt-1 flex flex-col gap-1.5 rounded-2xl border border-white/10 bg-neutral-950/85 p-3 shadow-2xl shadow-black/60 backdrop-blur-xl">
            <div className="text-[10px] font-semibold uppercase tracking-wider text-neutral-500">Themes</div>
            <div className="grid grid-cols-4 gap-1.5">
              {THEMES.map((t) => (
                <button
                  key={t.id}
                  onClick={() => onThemeChange(t.id)}
                  className={`group flex flex-col items-center gap-1.5 rounded-xl p-2 transition ${
                    themeId === t.id ? 'bg-white/10 ring-1 ring-white/20' : 'hover:bg-white/5'
                  }`}
                >
                  <div
                    className="h-8 w-full rounded-lg"
                    style={{ background: `linear-gradient(135deg, ${t.bgInner}, ${t.bgMid}, ${t.bgOuter})` }}
                  />
                  <span className={`text-[10px] font-medium ${themeId === t.id ? 'text-neutral-100' : 'text-neutral-400'}`}>
                    {t.displayName}
                  </span>
                </button>
              ))}
            </div>
          </div>
        )}

        {/* Pacing panel */}
        {showPacing && (
          <div className="mt-1 flex flex-col gap-1.5 rounded-2xl border border-white/10 bg-neutral-950/85 p-3 shadow-2xl shadow-black/60 backdrop-blur-xl">
            <div className="text-[10px] font-semibold uppercase tracking-wider text-neutral-500">Pacing</div>
            <div className="flex gap-1.5">
              {(Object.keys(PACING_PRESETS) as PacingPreset[]).map((p) => (
                <button
                  key={p}
                  onClick={() => onPacingChange(p)}
                  className={`flex flex-col items-center gap-0.5 rounded-xl px-3 py-2 transition ${
                    pacing === p ? 'bg-teal-500/20 text-teal-300 ring-1 ring-teal-400/30' : 'text-neutral-400 hover:bg-white/5'
                  }`}
                >
                  <span className="text-xs font-semibold">{PACING_PRESETS[p].label}</span>
                  <span className="text-[9px] text-neutral-500">{PACING_PRESETS[p].description.split(',')[0]}</span>
                </button>
              ))}
            </div>
          </div>
        )}

        {/* Wallpaper Settings Panel */}
        {showSettings && (
          <div className="mt-1 flex w-80 flex-col gap-3 rounded-2xl border border-white/10 bg-neutral-950/90 p-4 shadow-2xl shadow-black/60 backdrop-blur-xl text-neutral-200">
            <div className="text-[10px] font-semibold uppercase tracking-wider text-neutral-500">Wallpaper Customization</div>

            {/* Max Population */}
            <div className="space-y-1">
              <div className="flex justify-between text-xs">
                <span className="text-neutral-400">Population Cap</span>
                <span className="font-mono text-cyan-300">{wallpaperSettings.maxPopulation}</span>
              </div>
              <input
                type="range"
                min={50}
                max={500}
                step={25}
                value={wallpaperSettings.maxPopulation}
                onChange={(e) => {
                  const val = parseInt(e.target.value);
                  updateSetting('maxPopulation', val);
                  sim.settings = applyQualityProfile(sim.settings, wallpaperSettings.quality, val);
                }}
                className="w-full accent-cyan-400"
              />
            </div>

            {/* Quality Preset */}
            <div className="space-y-1">
              <span className="text-xs text-neutral-400">Rendering Quality</span>
              <div className="flex gap-1.5">
                {(['low', 'medium', 'high'] as QualityPreset[]).map((q) => (
                  <button
                    key={q}
                    onClick={() => {
                      updateSetting('quality', q);
                      sim.settings = applyQualityProfile(sim.settings, q, wallpaperSettings.maxPopulation);
                    }}
                    className={`flex-1 rounded-xl py-1.5 text-xs font-semibold capitalize transition ${
                      wallpaperSettings.quality === q
                        ? 'bg-purple-500/25 text-purple-300 ring-1 ring-purple-400/40'
                        : 'bg-neutral-900 text-neutral-400 hover:bg-neutral-800'
                    }`}
                  >
                    {q}
                  </button>
                ))}
              </div>
            </div>

            {/* Mutation Rate */}
            <div className="space-y-1">
              <div className="flex justify-between text-xs">
                <span className="text-neutral-400">Mutation Rate</span>
                <span className="font-mono text-cyan-300">{(wallpaperSettings.mutationRate * 100).toFixed(0)}%</span>
              </div>
              <input
                type="range"
                min={0.01}
                max={0.5}
                step={0.01}
                value={wallpaperSettings.mutationRate}
                onChange={(e) => updateSetting('mutationRate', parseFloat(e.target.value))}
                className="w-full accent-cyan-400"
              />
            </div>

            {/* Auto Pause */}
            <div className="flex items-center justify-between pt-1 border-b border-white/10 pb-3">
              <span className="text-xs text-neutral-400">Pause on Gaming / Background</span>
              <button
                onClick={() => updateSetting('autoPause', !wallpaperSettings.autoPause)}
                className={`h-5 w-9 rounded-full transition relative ${
                  wallpaperSettings.autoPause ? 'bg-emerald-500' : 'bg-neutral-800'
                }`}
              >
                <span
                  className={`absolute top-0.5 h-4 w-4 rounded-full bg-white transition ${
                    wallpaperSettings.autoPause ? 'left-4.5 bg-white' : 'left-0.5 bg-neutral-400'
                  }`}
                />
              </button>
            </div>

            {/* Active Simulation Features Summary (Read-Only) */}
            <div className="pt-1">
              <div className="text-[10px] font-semibold uppercase tracking-wider text-neutral-500 mb-2">Active Simulation Features</div>
              <div className="flex flex-wrap gap-1.5 text-[11px]">
                <span className={`px-2 py-0.5 rounded-md border ${settings.biomes ? 'border-green-500/30 bg-green-500/10 text-green-300' : 'border-neutral-800 bg-neutral-900/60 text-neutral-500'}`}>Biomes</span>
                <span className={`px-2 py-0.5 rounded-md border ${settings.knowledgeNodes ? 'border-indigo-500/30 bg-indigo-500/10 text-indigo-300' : 'border-neutral-800 bg-neutral-900/60 text-neutral-500'}`}>Knowledge</span>
                <span className={`px-2 py-0.5 rounded-md border ${settings.structureBuilding ? 'border-sky-500/30 bg-sky-500/10 text-sky-300' : 'border-neutral-800 bg-neutral-900/60 text-neutral-500'}`}>Structures</span>
                <span className={`px-2 py-0.5 rounded-md border ${settings.diseaseEvents ? 'border-red-500/30 bg-red-500/10 text-red-300' : 'border-neutral-800 bg-neutral-900/60 text-neutral-500'}`}>Disease</span>
                <span className={`px-2 py-0.5 rounded-md border ${settings.socialBehavior ? 'border-purple-500/30 bg-purple-500/10 text-purple-300' : 'border-neutral-800 bg-neutral-900/60 text-neutral-500'}`}>Social</span>
                <span className={`px-2 py-0.5 rounded-md border ${settings.endlessGeneration ? 'border-amber-500/30 bg-amber-500/10 text-amber-300' : 'border-neutral-800 bg-neutral-900/60 text-neutral-500'}`}>Endless</span>
              </div>
            </div>

            {/* Read-Only Active Sim Systems Summary */}
            <div className="space-y-1.5 pt-1">
              <div className="text-[10px] font-semibold uppercase tracking-wider text-neutral-400">Active Simulation Systems</div>
              <div className="grid grid-cols-2 gap-1 text-[10px]">
                <div className="flex items-center justify-between rounded-lg bg-white/5 px-2 py-1">
                  <span className="text-neutral-400">Biomes</span>
                  <span className={settings.biomes ? 'text-emerald-400 font-bold' : 'text-neutral-500'}>{settings.biomes ? 'ON' : 'OFF'}</span>
                </div>
                <div className="flex items-center justify-between rounded-lg bg-white/5 px-2 py-1">
                  <span className="text-neutral-400">Nodes</span>
                  <span className={settings.knowledgeNodes ? 'text-emerald-400 font-bold' : 'text-neutral-500'}>{settings.knowledgeNodes ? 'ON' : 'OFF'}</span>
                </div>
                <div className="flex items-center justify-between rounded-lg bg-white/5 px-2 py-1">
                  <span className="text-neutral-400">Structures</span>
                  <span className={settings.structureBuilding ? 'text-emerald-400 font-bold' : 'text-neutral-500'}>{settings.structureBuilding ? 'ON' : 'OFF'}</span>
                </div>
                <div className="flex items-center justify-between rounded-lg bg-white/5 px-2 py-1">
                  <span className="text-neutral-400">Colonies</span>
                  <span className={settings.colonyFormation ? 'text-emerald-400 font-bold' : 'text-neutral-500'}>{settings.colonyFormation ? 'ON' : 'OFF'}</span>
                </div>
                <div className="flex items-center justify-between rounded-lg bg-white/5 px-2 py-1">
                  <span className="text-neutral-400">Mating</span>
                  <span className={settings.sexualReproduction ? 'text-emerald-400 font-bold' : 'text-neutral-500'}>{settings.sexualReproduction ? 'ON' : 'OFF'}</span>
                </div>
                <div className="flex items-center justify-between rounded-lg bg-white/5 px-2 py-1">
                  <span className="text-neutral-400">Biology</span>
                  <span className={settings.advancedBiology ? 'text-emerald-400 font-bold' : 'text-neutral-500'}>{settings.advancedBiology ? 'ON' : 'OFF'}</span>
                </div>
                <div className="flex items-center justify-between rounded-lg bg-white/5 px-2 py-1">
                  <span className="text-neutral-400">Disease</span>
                  <span className={settings.diseaseEvents ? 'text-amber-400 font-bold' : 'text-neutral-500'}>{settings.diseaseEvents ? 'ON' : 'OFF'}</span>
                </div>
                <div className="flex items-center justify-between rounded-lg bg-white/5 px-2 py-1">
                  <span className="text-neutral-400">Social</span>
                  <span className={settings.socialBehavior ? 'text-emerald-400 font-bold' : 'text-neutral-500'}>{settings.socialBehavior ? 'ON' : 'OFF'}</span>
                </div>
              </div>
            </div>
          </div>
        )}
      </div>
      )}
    </div>
  );
}

function DockButton({
  onClick, active, icon, label, activeClass = '',
}: {
  onClick: () => void;
  active?: boolean;
  icon: React.ReactNode;
  label: string;
  activeClass?: string;
}) {
  return (
    <button
      onClick={onClick}
      title={label}
      className={`flex h-9 w-9 items-center justify-center rounded-xl text-neutral-300 transition hover:bg-neutral-800/80 active:scale-90 ${
        active ? activeClass : ''
      }`}
    >
      {icon}
    </button>
  );
}

function DockToggle({
  active, onClick, icon, label, activeClass,
}: {
  active: boolean;
  onClick: () => void;
  icon: React.ReactNode;
  label: string;
  activeClass: string;
}) {
  return (
    <button
      onClick={onClick}
      title={label}
      className={`flex h-9 w-9 items-center justify-center rounded-xl text-neutral-500 transition hover:bg-neutral-800/80 active:scale-90 ${
        active ? activeClass : 'opacity-50'
      }`}
    >
      {icon}
    </button>
  );
}

function Divider() {
  return <div className="h-6 w-px bg-white/10" />;
}
