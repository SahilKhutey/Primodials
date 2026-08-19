import type { SimSettings } from "@/sim/types";
import type { QualityPreset } from "@/hooks/useWallpaperSettings";

export type QualityProfile = {
  maxPopulation: number;
  maxFood: number;
  renderScale: number;
  simulationHz: number;
  showSense: boolean;
  showColonies: boolean;
  particleDensity: number;
};

export const QUALITY_PROFILES: Record<QualityPreset, QualityProfile> = {
  low: {
    maxPopulation: 125,
    maxFood: 180,
    renderScale: 0.75,
    simulationHz: 20,
    showSense: false,
    showColonies: false,
    particleDensity: 0.4,
  },
  medium: {
    maxPopulation: 250,
    maxFood: 300,
    renderScale: 0.9,
    simulationHz: 30,
    showSense: false,
    showColonies: true,
    particleDensity: 0.7,
  },
  high: {
    maxPopulation: 500,
    maxFood: 500,
    renderScale: 1,
    simulationHz: 30,
    showSense: true,
    showColonies: true,
    particleDensity: 1,
  },
};

export function applyQualityProfile(
  settings: SimSettings,
  quality: QualityPreset,
  populationOverride?: number,
): SimSettings {
  const profile = QUALITY_PROFILES[quality];
  return {
    ...settings,
    maxPopulation: Math.min(
      settings.maxPopulation,
      populationOverride ?? profile.maxPopulation,
    ),
    maxFood: Math.min(settings.maxFood, profile.maxFood),
  };
}
