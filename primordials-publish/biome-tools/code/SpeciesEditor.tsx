/**
 * Primordials — Species Parameter Inspector (React 18)
 *
 * License: MIT
 */

import React from 'react';
import type { SpeciesConfig, Biome } from '../../web-app/src/types/biome';

interface SpeciesEditorProps {
  species: SpeciesConfig;
  biome: Biome;
  onUpdate: (species: SpeciesConfig) => void;
}

export function SpeciesEditor({ species, biome, onUpdate }: SpeciesEditorProps) {
  const handleChange = (fields: Partial<SpeciesConfig>) => {
    onUpdate({ ...species, ...fields });
  };

  const handleBehaviorChange = (behaviorFields: any) => {
    onUpdate({
      ...species,
      behavior: { ...species.behavior, ...behaviorFields },
    });
  };

  return (
    <div className="space-y-4 text-xs">
      <h3 className="font-mono text-xs font-semibold uppercase tracking-wider text-text-muted">Species Attributes</h3>

      {/* Name & Type */}
      <div>
        <label className="block text-text-muted">Name</label>
        <input
          type="text"
          value={species.name}
          onChange={(e) => handleChange({ name: e.target.value })}
          className="mt-1 w-full rounded border border-bg-surface bg-bg-void px-3 py-1.5 focus:border-accent-magenta focus:outline-none"
        />
      </div>

      <div>
        <label className="block text-text-muted">Trophic Role</label>
        <div className="mt-1 grid grid-cols-2 gap-1">
          {(['producer', 'grazer', 'predator', 'scavenger'] as const).map((t) => (
            <button
              key={t}
              onClick={() => handleChange({ type: t })}
              className={`rounded px-2 py-1 capitalize transition-all ${species.type === t ? 'bg-accent-magenta text-bg-void font-semibold' : 'bg-bg-surface text-text-muted hover:text-text-primary'}`}
            >
              {t}
            </button>
          ))}
        </div>
      </div>

      {/* Color Hex & Size */}
      <div className="grid grid-cols-2 gap-2">
        <div>
          <label className="block text-text-muted">Color</label>
          <input
            type="text"
            value={species.color}
            onChange={(e) => handleChange({ color: e.target.value })}
            className="mt-1 w-full rounded border border-bg-surface bg-bg-void px-2 py-1 font-mono focus:border-accent-magenta focus:outline-none"
          />
        </div>
        <div>
          <label className="block text-text-muted">Size ({species.size}px)</label>
          <input
            type="range"
            min={2}
            max={16}
            value={species.size}
            onChange={(e) => handleChange({ size: parseInt(e.target.value) })}
            className="mt-2 w-full"
          />
        </div>
      </div>

      {/* Movement Speed */}
      <div>
        <div className="flex justify-between text-text-muted">
          <span>Speed</span>
          <span className="font-mono">{species.behavior.movement.speed}</span>
        </div>
        <input
          type="range"
          min={0.5}
          max={40}
          step={0.5}
          value={species.behavior.movement.speed}
          onChange={(e) =>
            handleBehaviorChange({
              movement: { ...species.behavior.movement, speed: parseFloat(e.target.value) },
            })
          }
          className="mt-1 w-full"
        />
      </div>

      {/* Reproduction Rate */}
      <div>
        <div className="flex justify-between text-text-muted">
          <span>Reproduction Rate</span>
          <span className="font-mono">{species.behavior.reproduction.rate}</span>
        </div>
        <input
          type="range"
          min={0.005}
          max={0.1}
          step={0.005}
          value={species.behavior.reproduction.rate}
          onChange={(e) =>
            handleBehaviorChange({
              reproduction: { ...species.behavior.reproduction, rate: parseFloat(e.target.value) },
            })
          }
          className="mt-1 w-full"
        />
      </div>

      {/* Initial Population */}
      <div>
        <div className="flex justify-between text-text-muted">
          <span>Initial Population</span>
          <span className="font-mono">{species.initial}</span>
        </div>
        <input
          type="range"
          min={0}
          max={1000}
          step={10}
          value={species.initial}
          onChange={(e) => handleChange({ initial: parseInt(e.target.value) })}
          className="mt-1 w-full"
        />
      </div>
    </div>
  );
}
