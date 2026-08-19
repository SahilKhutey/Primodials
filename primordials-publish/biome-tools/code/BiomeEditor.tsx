/**
 * Primordials — Main Biome Authoring Workspace (React 18)
 *
 * License: MIT
 */

import React, { useState, useEffect } from 'react';
import type { Biome } from '../../web-app/src/types/biome';
import { algaeBloom } from '../../web-app/src/biomes/algaeBloom';
import { SpeciesEditor } from './SpeciesEditor';
import { LivePreview } from './LivePreview';
import { validationEngine } from './ValidationEngine';
import { downloadJson, generateGitHubPRUrl } from './JsonExporter';
import { ArrowLeft, Download, GitPullRequest, Code, Eye, Sliders, Plus, Trash2 } from 'lucide-react';

export function BiomeEditor() {
  const [biome, setBiome] = useState<Biome>(algaeBloom);
  const [selectedSpeciesId, setSelectedSpeciesId] = useState<string>(biome.species[0]?.id ?? '');
  const [mode, setMode] = useState<'edit' | 'preview' | 'json'>('edit');
  const [activeTab, setActiveTab] = useState<'species' | 'world' | 'rules' | 'visuals'>('species');

  const validation = validationEngine.validate(biome);

  const selectedSpecies = biome.species.find((s) => s.id === selectedSpeciesId) ?? biome.species[0];

  const handleUpdateSpecies = (updated: any) => {
    setBiome((prev) => ({
      ...prev,
      species: prev.species.map((s) => (s.id === updated.id ? updated : s)),
    }));
  };

  const handleAddSpecies = () => {
    const newId = `species-${Date.now().toString().slice(-4)}`;
    const newSpecies = {
      id: newId,
      name: 'New Organism',
      type: 'grazer' as const,
      color: '#FF6B6B',
      shape: 'circle' as const,
      size: 5,
      initial: 50,
      behavior: {
        movement: { type: 'wander' as const, speed: 10.0, turnRate: 2.0 },
        feeding: { diet: [biome.species[0]?.id ?? 'algae'], consumptionRate: 0.05, hungerRate: 0.02, maxHunger: 1.0 },
        reproduction: { rate: 0.02, threshold: 0.6, offspring: 1, maturityAge: 20 },
      },
      stats: { maxEnergy: 1.0, metabolism: 0.01, lifespan: 200, vision: 50 },
    };
    setBiome((prev) => ({ ...prev, species: [...prev.species, newSpecies] }));
    setSelectedSpeciesId(newId);
  };

  return (
    <div className="flex h-screen w-screen flex-col overflow-hidden bg-bg-void text-text-primary">
      {/* Top Navigation */}
      <header className="flex h-14 items-center justify-between border-b border-bg-surface bg-bg-deep px-4">
        <div className="flex items-center gap-3">
          <input
            type="text"
            value={biome.name}
            onChange={(e) => setBiome({ ...biome, name: e.target.value })}
            className="rounded border border-transparent bg-transparent px-2 py-1 font-display text-lg font-bold hover:border-bg-surface focus:border-accent-magenta focus:outline-none"
          />
          <span className="font-mono text-xs text-text-muted">v{biome.version}</span>
        </div>

        {/* View Mode Switchers */}
        <div className="flex rounded-md bg-bg-surface p-1">
          <button
            onClick={() => setMode('edit')}
            className={`flex items-center gap-1.5 rounded px-3 py-1 text-xs font-medium ${mode === 'edit' ? 'bg-accent-magenta text-bg-void' : 'text-text-muted hover:text-text-primary'}`}
          >
            <Sliders className="h-3.5 w-3.5" /> Visual Edit
          </button>
          <button
            onClick={() => setMode('preview')}
            className={`flex items-center gap-1.5 rounded px-3 py-1 text-xs font-medium ${mode === 'preview' ? 'bg-accent-magenta text-bg-void' : 'text-text-muted hover:text-text-primary'}`}
          >
            <Eye className="h-3.5 w-3.5" /> Live Preview
          </button>
          <button
            onClick={() => setMode('json')}
            className={`flex items-center gap-1.5 rounded px-3 py-1 text-xs font-medium ${mode === 'json' ? 'bg-accent-magenta text-bg-void' : 'text-text-muted hover:text-text-primary'}`}
          >
            <Code className="h-3.5 w-3.5" /> Raw Schema
          </button>
        </div>

        {/* Actions */}
        <div className="flex items-center gap-2">
          <button
            onClick={() => downloadJson(biome)}
            disabled={!validation.isValid}
            className="flex items-center gap-1.5 rounded bg-bg-surface px-3 py-1.5 text-xs font-medium hover:bg-bg-surface/80 disabled:opacity-50"
          >
            <Download className="h-3.5 w-3.5" /> Export JSON
          </button>
          <a
            href={generateGitHubPRUrl(biome)}
            target="_blank"
            rel="noreferrer"
            className="flex items-center gap-1.5 rounded bg-accent-magenta px-3 py-1.5 text-xs font-semibold text-bg-void"
          >
            <GitPullRequest className="h-3.5 w-3.5" /> Submit PR
          </a>
        </div>
      </header>

      {/* Main Workspace */}
      <div className="flex flex-1 overflow-hidden">
        {mode === 'edit' && (
          <>
            {/* Left Species Roster */}
            <aside className="flex w-64 flex-col border-r border-bg-surface bg-bg-deep/70">
              <div className="flex items-center justify-between border-b border-bg-surface p-3">
                <span className="font-mono text-xs font-semibold uppercase tracking-wider text-text-muted">Species Roster</span>
                <button onClick={handleAddSpecies} className="rounded p-1 text-text-muted hover:bg-bg-surface hover:text-text-primary">
                  <Plus className="h-4 w-4" />
                </button>
              </div>
              <div className="flex-1 overflow-y-auto p-2 space-y-1">
                {biome.species.map((s) => (
                  <button
                    key={s.id}
                    onClick={() => setSelectedSpeciesId(s.id)}
                    className={`flex w-full items-center gap-2 rounded px-3 py-2 text-left text-xs transition-all ${s.id === selectedSpeciesId ? 'bg-accent-magenta/15 text-text-primary ring-1 ring-accent-magenta' : 'text-text-muted hover:bg-bg-surface hover:text-text-primary'}`}
                  >
                    <div className="h-3 w-3 rounded-full" style={{ backgroundColor: s.color }} />
                    <span className="flex-1 font-medium">{s.name}</span>
                    <span className="font-mono opacity-60">×{s.initial}</span>
                  </button>
                ))}
              </div>
            </aside>

            {/* Center Live Simulation */}
            <main className="relative flex-1">
              <LivePreview biome={biome} />
            </main>

            {/* Right Inspector */}
            <aside className="w-80 overflow-y-auto border-l border-bg-surface bg-bg-deep/70 p-4">
              {selectedSpecies && <SpeciesEditor species={selectedSpecies} biome={biome} onUpdate={handleUpdateSpecies} />}
            </aside>
          </>
        )}

        {mode === 'preview' && (
          <main className="relative flex-1">
            <LivePreview biome={biome} />
          </main>
        )}

        {mode === 'json' && (
          <main className="flex-1 overflow-auto bg-bg-void p-6 font-mono text-xs text-text-primary">
            <pre>{JSON.stringify(biome, null, 2)}</pre>
          </main>
        )}
      </div>

      {/* Footer Diagnostic HUD */}
      <footer className="flex h-8 items-center justify-between border-t border-bg-surface bg-bg-deep px-4 font-mono text-xs text-text-muted">
        <div>
          {validation.isValid ? (
            <span className="text-accent-teal">✅ Schema Valid ({biome.species.length} species)</span>
          ) : (
            <span className="text-accent-coral">⚠️ {validation.errors.length} Error(s) Blocking Export</span>
          )}
        </div>
        <div>Primordials Ecosystem Authoring Suite v2.1.0</div>
      </footer>
    </div>
  );
}
