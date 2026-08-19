/**
 * Primordials — JSON Export and GitHub PR Automation
 *
 * License: MIT
 */

import type { Biome } from '../../web-app/src/types/biome';

export function exportToJson(biome: Biome): string {
  return JSON.stringify(biome, null, 2);
}

export function downloadJson(biome: Biome, filename?: string): void {
  const json = exportToJson(biome);
  const blob = new Blob([json], { type: 'application/json' });
  const url = URL.createObjectURL(blob);
  const link = document.createElement('a');
  link.href = url;
  link.download = filename ?? `${biome.id}.json`;
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
  URL.revokeObjectURL(url);
}

export function generateGitHubPRUrl(biome: Biome): string {
  const json = encodeURIComponent(exportToJson(biome));
  const title = encodeURIComponent(`🌿 New Biome: ${biome.name}`);
  const body = encodeURIComponent(
    `### 🌿 Community Biome Submission: ${biome.name}\n\n` +
    `${biome.description}\n\n` +
    `**Species:** ${biome.species.length}\n` +
    `**Author:** @${biome.author || 'community'}\n\n` +
    `Generated via Primordials Biome Authoring Suite.`
  );

  return `https://github.com/SahilKhutey/Primodials/new/main/biomes?filename=${biome.id}.json&value=${json}&message=${title}&description=${body}`;
}
