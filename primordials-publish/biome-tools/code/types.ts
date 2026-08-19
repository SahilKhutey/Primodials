/**
 * Primordials — Biome Authoring Tool Type Definitions
 *
 * License: MIT
 */

import type { Biome, SpeciesConfig } from '../../web-app/src/types/biome';

export interface ValidationIssue {
  code: string;
  message: string;
  severity: 'error' | 'warning' | 'info';
  path?: string;
  speciesId?: string;
}

export interface ValidationResult {
  errors: ValidationIssue[];
  warnings: ValidationIssue[];
  info: ValidationIssue[];
  isValid: boolean;
}

export interface EditorState {
  biome: Biome;
  selectedSpeciesId: string | null;
  selectedTab: 'species' | 'world' | 'rules' | 'visuals';
  mode: 'edit' | 'preview' | 'json';
  isDirty: boolean;
}

export interface GalleryBiome {
  id: string;
  name: string;
  description: string;
  author: string;
  biome: Biome;
  createdAt: string;
  downloads: number;
  rating: number;
  tags: string[];
}
