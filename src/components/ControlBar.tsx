import {
  Play, Pause, RotateCcw, Eye, Sparkles, Users, Mountain, Lightbulb,
  Sparkle, Home, Heart, Box, ChevronRight, Brain, Clock, Globe, Save, FlaskConical, Beaker, Infinity as InfinityIcon, Shield, Leaf, UsersRound, Network,
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

      {/* Advanced simulation toggles — real intelligence & endless life */}
      <div className="flex flex-wrap items-center gap-2">
        <span className="text-xs font-semibold uppercase tracking-wider text-neutral-500">Advanced:</span>
        <ToggleButton
          active={settings.neuralBrains}
          onClick={() => onToggleSetting('neuralBrains', !settings.neuralBrains)}
          icon={<Brain size={15} />}
          label="Neural Brains"
          activeClass="bg-fuchsia-500/20 text-fuchsia-300"
        />
        <ToggleButton
          active={settings.worldExpansion}
          onClick={() => onToggleSetting('worldExpansion', !settings.worldExpansion)}
          icon={<Globe size={15} />}
          label="World Expansion"
          activeClass="bg-emerald-500/20 text-emerald-300"
        />
        <ToggleButton
          active={settings.autoCheckpoint}
          onClick={() => onToggleSetting('autoCheckpoint', !settings.autoCheckpoint)}
          icon={<Save size={15} />}
          label="Auto-Checkpoint"
          activeClass="bg-blue-500/20 text-blue-300"
        />
        <div className="flex items-center gap-1.5 rounded-lg bg-neutral-800/80 px-3 py-2 ring-1 ring-white/5">
          <Clock size={15} className="text-amber-300" />
          <span className="text-xs font-semibold text-neutral-400">Gen Cycle:</span>
          <span className="text-xs font-bold text-amber-300">{settings.generationCycleTicks}t</span>
        </div>
      </div>

      {/* Microbial evolution toggles */}
      <div className="flex flex-wrap items-center gap-2">
        <span className="text-xs font-semibold uppercase tracking-wider text-neutral-500">Microbial:</span>
        <ToggleButton
          active={settings.chemicalField}
          onClick={() => onToggleSetting('chemicalField', !settings.chemicalField)}
          icon={<FlaskConical size={15} />}
          label="Chemical Field"
          activeClass="bg-lime-500/20 text-lime-300"
        />
        <ToggleButton
          active={settings.microbialBehavior}
          onClick={() => onToggleSetting('microbialBehavior', !settings.microbialBehavior)}
          icon={<Beaker size={15} />}
          label="Microbial Behavior"
          activeClass="bg-green-500/20 text-green-300"
        />
      </div>

      {/* Advanced evolution toggles */}
      <div className="flex flex-wrap items-center gap-2">
        <span className="text-xs font-semibold uppercase tracking-wider text-neutral-500">Advanced:</span>
        <ToggleButton
          active={settings.endlessGeneration}
          onClick={() => onToggleSetting('endlessGeneration', !settings.endlessGeneration)}
          icon={<InfinityIcon size={15} />}
          label="Endless Generation"
          activeClass="bg-amber-500/20 text-amber-300"
        />
        <ToggleButton
          active={settings.advancedBiology}
          onClick={() => onToggleSetting('advancedBiology', !settings.advancedBiology)}
          icon={<Leaf size={15} />}
          label="Advanced Biology"
          activeClass="bg-emerald-500/20 text-emerald-300"
        />
        <ToggleButton
          active={settings.diseaseEvents}
          onClick={() => onToggleSetting('diseaseEvents', !settings.diseaseEvents)}
          icon={<Shield size={15} />}
          label="Disease"
          activeClass="bg-red-500/20 text-red-300"
        />
        <ToggleButton
          active={settings.socialBehavior}
          onClick={() => onToggleSetting('socialBehavior', !settings.socialBehavior)}
          icon={<UsersRound size={15} />}
          label="Social Behavior"
          activeClass="bg-indigo-500/20 text-indigo-300"
        />
        <ToggleButton
          active={settings.neutralDrift}
          onClick={() => onToggleSetting('neutralDrift', !settings.neutralDrift)}
          icon={<Network size={15} />}
          label="Neutral Drift"
          activeClass="bg-slate-500/20 text-slate-300"
        />
      </div>
    </div>
  );
}
