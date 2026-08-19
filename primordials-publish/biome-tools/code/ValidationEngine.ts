/**
 * Primordials — Real-Time Biome Diagnostic Validation Engine
 *
 * License: MIT
 */

import type { Biome } from '../../web-app/src/types/biome';
import type { ValidationIssue, ValidationResult } from './types';

const BRAND_PALETTE = new Set([
  '#0F3D3E', '#0A0908', '#FF4FA3', '#F4F1DE', '#A8B5A0',
  '#1A4D4E', '#4ECDC4', '#FF6B6B', '#FFD93D', '#6BCB77',
  '#F18F01', '#D62246', '#1B998B', '#FFE66D', '#4A4E69',
  '#9B5DE5', '#FF9F1C', '#E63946', '#4D9DE0', '#E15554',
]);

export class ValidationEngine {
  validate(biome: Biome): ValidationResult {
    const errors: ValidationIssue[] = [];
    const warnings: ValidationIssue[] = [];
    const info: ValidationIssue[] = [];

    this.validateSchema(biome, errors);
    this.validateSpecies(biome, errors, warnings);
    this.validateWorld(biome, errors, warnings);
    this.validateRules(biome, errors, warnings);
    this.validateVisuals(biome, errors, warnings);
    this.validateDynamics(biome, warnings, info);

    return {
      errors,
      warnings,
      info,
      isValid: errors.length === 0,
    };
  }

  private validateSchema(biome: Biome, errors: ValidationIssue[]): void {
    if (!biome.id) {
      errors.push({ code: 'E001', severity: 'error', message: 'Missing biome ID' });
    } else if (!/^[a-z0-9-]+$/.test(biome.id)) {
      errors.push({ code: 'E002', severity: 'error', message: 'Biome ID must be kebab-case' });
    }
    if (!biome.species || biome.species.length === 0) {
      errors.push({ code: 'E005', severity: 'error', message: 'Biome must define at least one species' });
    }
  }

  private validateSpecies(biome: Biome, errors: ValidationIssue[], warnings: ValidationIssue[]): void {
    const ids = new Set<string>();
    let hasProducer = false;

    biome.species.forEach((s, idx) => {
      if (ids.has(s.id)) {
        errors.push({ code: 'E102', severity: 'error', message: `Duplicate species ID: ${s.id}` });
      }
      ids.add(s.id);

      if (s.type === 'producer') hasProducer = true;

      if (!/^#[0-9A-Fa-f]{6}$/.test(s.color)) {
        errors.push({ code: 'E106', severity: 'error', message: `Invalid hex color code: ${s.color}` });
      } else if (!BRAND_PALETTE.has(s.color.toUpperCase())) {
        warnings.push({ code: 'W101', severity: 'warning', message: `Species color ${s.color} not in brand palette` });
      }

      if (s.behavior.reproduction.rate > 0.1) {
        warnings.push({ code: 'W204', severity: 'warning', message: `${s.name} reproduction rate is very high` });
      }
    });

    if (!hasProducer && biome.species.length > 0) {
      errors.push({ code: 'E006', severity: 'error', message: 'Biome must have at least one producer species' });
    }

    biome.species.forEach((s) => {
      s.behavior.feeding.diet.forEach((dietId) => {
        if (!ids.has(dietId)) {
          errors.push({ code: 'E111', severity: 'error', message: `Diet references unknown species: ${dietId}` });
        }
        if (dietId === s.id) {
          errors.push({ code: 'E112', severity: 'error', message: 'Species cannot eat itself' });
        }
      });
      if ((s.type === 'predator' || s.type === 'grazer') && s.behavior.feeding.diet.length === 0) {
        errors.push({ code: 'E113', severity: 'error', message: `${s.name} has no prey configured in diet` });
      }
      if (s.type === 'producer' && s.behavior.feeding.diet.length > 0) {
        errors.push({ code: 'E114', severity: 'error', message: 'Producer species cannot have a diet' });
      }
    });
  }

  private validateWorld(biome: Biome, errors: ValidationIssue[], warnings: ValidationIssue[]): void {
    if (biome.world.width <= 0 || biome.world.height <= 0) {
      errors.push({ code: 'E203', severity: 'error', message: 'World dimensions must be positive' });
    }
  }

  private validateRules(biome: Biome, errors: ValidationIssue[], warnings: ValidationIssue[]): void {
    const totalInitial = biome.species.reduce((acc, s) => acc + s.initial, 0);
    if (biome.rules.carryingCapacity < totalInitial) {
      errors.push({ code: 'E202', severity: 'error', message: `Carrying capacity (${biome.rules.carryingCapacity}) < initial count (${totalInitial})` });
    }
  }

  private validateVisuals(biome: Biome, errors: ValidationIssue[], warnings: ValidationIssue[]): void {
    if (!/^#[0-9A-Fa-f]{6}$/.test(biome.visuals.background)) {
      errors.push({ code: 'E301', severity: 'error', message: 'Invalid background hex color' });
    }
  }

  private validateDynamics(biome: Biome, warnings: ValidationIssue[], info: ValidationIssue[]): void {
    if (biome.species.length < 3) {
      info.push({ code: 'I001', severity: 'info', message: 'Consider adding 3+ species for richer ecological cycles' });
    }
  }
}

export const validationEngine = new ValidationEngine();
