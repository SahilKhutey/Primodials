/**
 * Primordials — Community Ecosystem Gallery (React 18)
 *
 * License: MIT
 */

import React, { useState } from 'react';
import type { GalleryBiome } from './types';
import { algaeBloom } from '../../web-app/src/biomes/algaeBloom';
import { deepSea } from '../../web-app/src/biomes/deepSea';
import { Download, GitFork, Star, Search } from 'lucide-react';

const SAMPLE_COMMUNITY_BIOMES: GalleryBiome[] = [
  {
    id: 'abyssal-kelp',
    name: 'Abyssal Kelp Forest',
    description: 'Dense deep-sea kelp forest dominated by giant bioluminescent nudibranchs.',
    author: 'ocean_coder',
    biome: deepSea,
    createdAt: '2026-08-10',
    downloads: 342,
    rating: 4.8,
    tags: ['deepsea', 'kelp', 'bioluminescent'],
  },
  {
    id: 'cyanobacteria-soup',
    name: 'Cyanobacteria Soup',
    description: 'Chaotic Precambrian oceanic thermal vent producing intense oxygen pulses.',
    author: 'geobiologist',
    biome: algaeBloom,
    createdAt: '2026-08-15',
    downloads: 215,
    rating: 4.6,
    tags: ['ancient', 'geothermal', 'chaotic'],
  },
];

export function CommunityGallery() {
  const [search, setSearch] = useState('');

  const filtered = SAMPLE_COMMUNITY_BIOMES.filter(
    (b) =>
      b.name.toLowerCase().includes(search.toLowerCase()) ||
      b.tags.some((t) => t.toLowerCase().includes(search.toLowerCase()))
  );

  return (
    <div className="min-h-screen bg-bg-void p-8 text-text-primary">
      <header className="mb-8">
        <h1 className="font-display text-3xl font-bold">Community Biome Gallery</h1>
        <p className="text-sm text-text-muted">Discover, download, and fork ecosystems created by the Primordials community.</p>

        <div className="relative mt-4 max-w-md">
          <Search className="absolute left-3 top-2.5 h-4 w-4 text-text-muted" />
          <input
            type="text"
            placeholder="Search biomes, tags, authors..."
            value={search}
            onChange={(e) => setSearch(e.target.value)}
            className="w-full rounded-md border border-bg-surface bg-bg-deep pl-9 pr-4 py-2 text-xs focus:border-accent-magenta focus:outline-none"
          />
        </div>
      </header>

      <div className="grid gap-6 sm:grid-cols-2 lg:grid-cols-3">
        {filtered.map((item) => (
          <div key={item.id} className="rounded-lg border border-bg-surface bg-bg-deep/80 p-5 backdrop-blur-md">
            <div className="flex items-start justify-between">
              <div>
                <h3 className="font-bold">{item.name}</h3>
                <span className="font-mono text-xs text-text-muted">by @{item.author}</span>
              </div>
              <div className="flex items-center gap-1 text-xs text-accent-amber font-mono">
                <Star className="h-3.5 w-3.5 fill-current" />
                {item.rating}
              </div>
            </div>

            <p className="my-3 text-xs text-text-muted">{item.description}</p>

            <div className="flex flex-wrap gap-1 mb-4">
              {item.tags.map((tag) => (
                <span key={tag} className="rounded-full bg-bg-surface px-2 py-0.5 font-mono text-[10px] text-text-muted">
                  #{tag}
                </span>
              ))}
            </div>

            <div className="flex items-center justify-between border-t border-bg-surface/50 pt-3">
              <span className="font-mono text-xs text-text-muted">{item.downloads} downloads</span>
              <button className="flex items-center gap-1 rounded bg-bg-surface px-3 py-1 text-xs font-semibold hover:bg-accent-magenta hover:text-bg-void transition-colors">
                <GitFork className="h-3 w-3" /> Fork Biome
              </button>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
}
