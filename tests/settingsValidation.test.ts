/**
 * settingsValidation.test.ts — Phase 1 tests
 *
 * Vitest. Run with: npm run test
 *
 * License: MIT
 */

import { describe, it, expect } from 'vitest';
import {
  validateSettings,
  DEFAULT_SETTINGS,
  SETTINGS_BOUNDS,
} from '../src/lib/settingsValidation';

describe('validateSettings', () => {
  it('returns defaults for null', () => {
    const out = validateSettings(null);
    expect(out).toEqual(DEFAULT_SETTINGS);
  });

  it('returns defaults for undefined', () => {
    const out = validateSettings(undefined);
    expect(out).toEqual(DEFAULT_SETTINGS);
  });

  it('returns defaults for non-object input', () => {
    expect(validateSettings(42)).toEqual(DEFAULT_SETTINGS);
    expect(validateSettings('hello')).toEqual(DEFAULT_SETTINGS);
    expect(validateSettings([])).toEqual(DEFAULT_SETTINGS);
  });

  it('accepts a valid input unchanged', () => {
    const out = validateSettings({ ...DEFAULT_SETTINGS });
    expect(out).toEqual(DEFAULT_SETTINGS);
  });

  it('clamps out-of-range numbers to bounds', () => {
    const out = validateSettings({
      ...DEFAULT_SETTINGS,
      worldWidth: 999999,       // > max
      initialPopulation: -5,    // < min
      mutationRate: 2.5,        // > max
    });
    expect(out.worldWidth).toBe(SETTINGS_BOUNDS.worldWidth.max);
    expect(out.initialPopulation).toBe(SETTINGS_BOUNDS.initialPopulation.min);
    expect(out.mutationRate).toBe(SETTINGS_BOUNDS.mutationRate.max);
  });

  it('replaces NaN with the default', () => {
    const out = validateSettings({
      ...DEFAULT_SETTINGS,
      worldWidth: NaN,
      initialPopulation: NaN,
    });
    expect(Number.isFinite(out.worldWidth)).toBe(true);
    expect(Number.isFinite(out.initialPopulation)).toBe(true);
  });

  it('drops unknown keys', () => {
    const out = validateSettings({
      ...DEFAULT_SETTINGS,
      evilFlag: true,
      hacked: 999,
    });
    expect((out as Record<string, unknown>).evilFlag).toBeUndefined();
    expect((out as Record<string, unknown>).hacked).toBeUndefined();
  });

  it('forces maxPopulation >= initialPopulation', () => {
    const out = validateSettings({
      ...DEFAULT_SETTINGS,
      initialPopulation: 1000,
      maxPopulation: 500,
    });
    expect(out.maxPopulation).toBeGreaterThanOrEqual(out.initialPopulation);
  });

  it('drops non-boolean boolean fields', () => {
    const out = validateSettings({
      ...DEFAULT_SETTINGS,
      sexualReproduction: 'yes' as unknown as boolean,
    });
    expect(typeof out.sexualReproduction).toBe('boolean');
  });

  it('clamps negative foodGrowthRate to 0', () => {
    const out = validateSettings({
      ...DEFAULT_SETTINGS,
      foodGrowthRate: -1,
    });
    expect(out.foodGrowthRate).toBe(0);
  });

  it('survives deeply nested bad input', () => {
    const evil = {
      ...DEFAULT_SETTINGS,
      worldWidth: { nested: 'evil' } as unknown as number,
      initialPopulation: [1, 2, 3] as unknown as number,
    };
    const out = validateSettings(evil);
    expect(out.worldWidth).toBe(DEFAULT_SETTINGS.worldWidth);
    expect(out.initialPopulation).toBe(DEFAULT_SETTINGS.initialPopulation);
  });
});
