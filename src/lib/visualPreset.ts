import type { SimSettings } from "@/sim/types";

export type VisualPresetId = "wallpaper" | "cinematic" | "capture";

export type VisualPreset = {
  id: VisualPresetId;
  showSense: boolean;
  showColonies: boolean;
  renderScale: number;
  particleScale: number;
  uiOpacity: number;
};

export const VISUAL_PRESETS: Record<VisualPresetId, VisualPreset> = {
  wallpaper: { id: "wallpaper", showSense: false, showColonies: true, renderScale: 1, particleScale: 1, uiOpacity: 1 },
  cinematic: { id: "cinematic", showSense: false, showColonies: true, renderScale: 1, particleScale: 1.2, uiOpacity: 0.15 },
  capture: { id: "capture", showSense: false, showColonies: true, renderScale: 1.15, particleScale: 1.35, uiOpacity: 0 },
};

export function getVisualPreset(mode: VisualPresetId, settings: SimSettings): VisualPreset {
  const preset = VISUAL_PRESETS[mode];
  return {
    ...preset,
    showColonies: settings.socialBehavior && preset.showColonies,
  };
}
