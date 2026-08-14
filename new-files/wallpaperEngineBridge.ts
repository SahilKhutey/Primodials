import type { PacingPreset } from '@/sim/themes';
import { THEMES } from '@/sim/themes';
import type { SimSettings, BoundaryMode } from '@/sim/types';

// Bridges Wallpaper Engine's *native* right-hand settings panel (configured by
// the Workshop author inside the Wallpaper Engine editor's "Change Project
// settings" screen — see docs.wallpaperengine.io/en/web/customization/properties.html)
// to this app's existing settings handlers. Wallpaper Engine calls
// window.wallpaperPropertyListener.applyUserProperties(...) whenever the user
// changes a property, and once on load with every property's current value.
//
// This is purely additive: outside Wallpaper Engine, window.wallpaperPropertyListener
// is simply never invoked, so this module is a safe no-op. The in-content
// WallpaperDock keeps working standalone either way — this bridge lets Wallpaper
// Engine's own native panel drive the *same* underlying settings, so a user
// doesn't need to hover/interact with the wallpaper itself to change it.
//
// PROPERTY KEYS EXPECTED (create these in the Wallpaper Engine editor's project
// settings — the key names below must match exactly what you type in there):
//
//   theme            combo    options = each theme's `id` from src/sim/themes.ts
//                              (currently: deep-sea, coral-reef, aurora,
//                              microorganisms, forest-floor, crystal-cave)
//   pacing           combo    options = peaceful | lively | dynamic
//   chemicalField    bool
//   microbialBehavior bool
//   neuralBrains     bool
//   worldExpansion   bool
//
// Any property you don't create simply never fires — this bridge only acts on
// keys that are present in the `properties` object Wallpaper Engine sends.

type WallpaperEngineHandlers = {
  onThemeChange: (id: string) => void;
  onPacingChange: (pacing: PacingPreset) => void;
  onToggleSetting: (key: keyof SimSettings, value: boolean | BoundaryMode) => void;
};

type WallpaperEngineProperty = { value: string | number | boolean };
type WallpaperEngineProperties = Record<string, WallpaperEngineProperty>;

const VALID_THEME_IDS = new Set(THEMES.map((t) => t.id));
const VALID_PACING: ReadonlySet<PacingPreset> = new Set(['peaceful', 'lively', 'dynamic']);
const BOOL_SETTING_KEYS: readonly (keyof SimSettings)[] = [
  'chemicalField',
  'microbialBehavior',
  'neuralBrains',
  'worldExpansion',
];

export function installWallpaperEngineBridge(handlers: WallpaperEngineHandlers): void {
  window.wallpaperPropertyListener = {
    applyUserProperties(properties: WallpaperEngineProperties) {
      if (properties.theme && typeof properties.theme.value === 'string') {
        const id = properties.theme.value;
        if (VALID_THEME_IDS.has(id)) handlers.onThemeChange(id);
      }

      if (properties.pacing && typeof properties.pacing.value === 'string') {
        const pacing = properties.pacing.value as PacingPreset;
        if (VALID_PACING.has(pacing)) handlers.onPacingChange(pacing);
      }

      for (const key of BOOL_SETTING_KEYS) {
        const prop = properties[key as string];
        if (prop && typeof prop.value === 'boolean') {
          handlers.onToggleSetting(key, prop.value);
        }
      }
    },
  };
}

declare global {
  interface Window {
    wallpaperPropertyListener?: {
      applyUserProperties: (properties: WallpaperEngineProperties) => void;
    };
  }
}
