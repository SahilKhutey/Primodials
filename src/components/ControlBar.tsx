import {
  Play, Pause, RotateCcw, Eye, Sparkles, Users, Mountain, Lightbulb,
  Sparkle, Home, Heart, Box, ChevronRight,
} from 'lucide-react';
import type { SimSettings, BoundaryMode } from '@/sim/types';

type Props = {
  running: boolean;
  onToggleRun: () => void;
  onReset: () => void;
  showSense: boolean;
  onToggleSense: () => void;
  onSeedLife: () => void;
  showColonies: boolean;
  onToggleColonies: () => void;
  settings: SimSettings;
  onToggleSetting: (key: keyof SimSettings, value: boolean | BoundaryMode) => void;
};

function ToggleButton({
  active,
  onClick,
  icon,
  label,
  activeClass,
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
      className={`flex items-center gap-2 rounded-lg px-3.5 py-2 text-sm font-semibold transition active:scale-95 ${
        active
          ? `${activeClass} ring-1 ring-white/20`
          : 'bg-neutral-800/80 text-neutral-300 hover:bg-neutral-700/80 ring-1 ring-white/5'
      }`}
    >
      {icon}
      {label}
    </button>
  );
}

const BOUNDARY_MODES: BoundaryMode[] = ['wrap', 'reflect', 'open'];
const BOUNDARY_LABELS: Record<BoundaryMode, string> = {
  wrap: 'Endless',
  reflect: 'Reflect',
  open: 'Walled',
};

export function ControlBar({
  running,
  onToggleRun,
  onReset,
  showSense,
  onToggleSense,
  onSeedLife,
  showColonies,
  onToggleColonies,
  settings,
  onToggleSetting,
}: Props) {
  const cycleBoundary = () => {
    const idx = BOUNDARY_MODES.indexOf(settings.boundaryMode);
    const next = BOUNDARY_MODES[(idx + 1) % BOUNDARY_MODES.length];
    onToggleSetting('boundaryMode', next);
  };

  return (
    <div className="flex flex-col gap-2">
      <div className="flex flex-wrap items-center gap-2">
        <button
          onClick={onToggleRun}
          className={`flex items-center gap-2 rounded-lg px-5 py-2 text-sm font-bold transition active:scale-95 ${
            running
              ? 'bg-gradient-to-br from-amber-400 to-orange-500 text-neutral-900 shadow-lg shadow-amber-500/20'
              : 'bg-gradient-to-br from-emerald-400 to-teal-500 text-neutral-900 shadow-lg shadow-emerald-500/20'
          }`}
        >
          {running ? <Pause size={16} /> : <Play size={16} />}
          {running ? 'Pause' : 'Play'}
        </button>
        <button
          onClick={onReset}
          className="flex items-center gap-2 rounded-lg bg-neutral-800/80 px-4 py-2 text-sm font-semibold text-neutral-200 transition hover:bg-neutral-700/80 active:scale-95 ring-1 ring-white/5"
        >
          <RotateCcw size={16} />
          Reset
        </button>
        <button
          onClick={onSeedLife}
          className="flex items-center gap-2 rounded-lg bg-neutral-800/80 px-4 py-2 text-sm font-semibold text-neutral-200 transition hover:bg-neutral-700/80 active:scale-95 ring-1 ring-white/5"
        >
          <Sparkles size={16} />
          Seed Life
        </button>
        <div className="mx-1 h-8 w-px bg-neutral-700/60" />
        <ToggleButton
          active={showSense}
          onClick={onToggleSense}
          icon={<Eye size={16} />}
          label="Sense Radius"
          activeClass="bg-cyan-500/20 text-cyan-300"
        />
        <ToggleButton
          active={showColonies}
          onClick={onToggleColonies}
          icon={<Users size={16} />}
          label="Colonies"
          activeClass="bg-violet-500/20 text-violet-300"
        />
      </div>

      {/* Feature toggles — live simulation aspects */}
      <div className="flex flex-wrap items-center gap-2">
        <span className="text-xs font-semibold uppercase tracking-wider text-neutral-500">Features:</span>
        <ToggleButton
          active={settings.biomes}
          onClick={() => onToggleSetting('biomes', !settings.biomes)}
          icon={<Mountain size={15} />}
          label="Biomes"
          activeClass="bg-green-500/20 text-green-300"
        />
        <ToggleButton
          active={settings.knowledgeNodes}
          onClick={() => onToggleSetting('knowledgeNodes', !settings.knowledgeNodes)}
          icon={<Lightbulb size={15} />}
          label="Knowledge"
          activeClass="bg-indigo-500/20 text-indigo-300"
        />
        <ToggleButton
          active={settings.structureBuilding}
          onClick={() => onToggleSetting('structureBuilding', !settings.structureBuilding)}
          icon={<Home size={15} />}
          label="Structures"
          activeClass="bg-sky-500/20 text-sky-300"
        />
        <ToggleButton
          active={settings.blueprints}
          onClick={() => onToggleSetting('blueprints', !settings.blueprints)}
          icon={<Sparkle size={15} />}
          label="Blueprints"
          activeClass="bg-amber-500/20 text-amber-300"
        />
        <ToggleButton
          active={settings.sexualReproduction}
          onClick={() => onToggleSetting('sexualReproduction', !settings.sexualReproduction)}
          icon={<Heart size={15} />}
          label="Sexual Rep."
          activeClass="bg-pink-500/20 text-pink-300"
        />
        <ToggleButton
          active={settings.colonyFormation}
          onClick={() => onToggleSetting('colonyFormation', !settings.colonyFormation)}
          icon={<Users size={15} />}
          label="Colony Formation"
          activeClass="bg-teal-500/20 text-teal-300"
        />
        <button
          onClick={cycleBoundary}
          className="flex items-center gap-2 rounded-lg bg-neutral-800/80 px-3.5 py-2 text-sm font-semibold text-neutral-300 ring-1 ring-white/5 transition hover:bg-neutral-700/80 active:scale-95"
          title="Cycle world boundary mode"
        >
          <Box size={15} className="text-cyan-300" />
          <span className="text-neutral-400">Boundary:</span>
          <span className="text-cyan-300">{BOUNDARY_LABELS[settings.boundaryMode]}</span>
          <ChevronRight size={14} className="text-neutral-500" />
        </button>
      </div>
    </div>
  );
}
