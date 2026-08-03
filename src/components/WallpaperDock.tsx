import { useEffect, useRef, useState } from 'react';
import {
  Play, Pause, RotateCcw, Sparkles, Eye, FlaskConical, Beaker,
  Brain, Globe, Save, Maximize, Minimize, Camera,
} from 'lucide-react';
import type { Simulation } from '@/sim/simulation';
import type { SimSettings } from '@/sim/types';
import type { CinematicCamera } from '@/sim/cinematicCamera';

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
};

// Minimal floating dock for wallpaper mode — auto-hides after inactivity.
// Slides up from the bottom when the mouse moves near the bottom edge.
export function WallpaperDock({
  sim, running, onToggleRun, onReset, settings, onToggleSetting,
  cinematic, onToggleFullscreen, isFullscreen,
}: Props) {
  const [visible, setVisible] = useState(false);
  const [expanded, setExpanded] = useState(false);
  const hideTimer = useRef<number | null>(null);

  useEffect(() => {
    const onMove = (e: MouseEvent) => {
      if (e.clientY > window.innerHeight - 120) {
        setVisible(true);
        setExpanded(true);
        if (hideTimer.current) clearTimeout(hideTimer.current);
        hideTimer.current = window.setTimeout(() => {
          setExpanded(false);
        }, 2500);
        hideTimer.current = window.setTimeout(() => {
          setVisible(false);
        }, 4000);
      } else if (e.clientY < window.innerHeight - 200) {
        if (hideTimer.current) clearTimeout(hideTimer.current);
        hideTimer.current = window.setTimeout(() => {
          setVisible(false);
        }, 500);
      }
    };
    window.addEventListener('mousemove', onMove);
    return () => window.removeEventListener('mousemove', onMove);
  }, []);

  return (
    <div
      className={`fixed bottom-0 left-1/2 z-30 -translate-x-1/2 transition-all duration-500 ${
        visible ? 'translate-y-0 opacity-100' : 'translate-y-20 opacity-0'
      }`}
    >
      <div className={`mx-auto flex flex-col items-center gap-2 transition-all duration-300 ${expanded ? 'pb-4' : 'pb-2'}`}>
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
      </div>
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
