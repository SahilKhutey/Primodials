# Phase 2 — Apply to Primordials

## Files to copy

Copy all files into the repository preserving:

```text
src/
tests/
docs/phase2/
```

## Required dependencies

Phase 2 does not add a new runtime dependency.

It assumes Phase 1's Vitest setup exists.

## Minimal integration sequence

### A. Wallpaper auto-pause

In `App.tsx`, import:

```ts
import { installVisibilityController } from "@/lib/visibilityController";
```

Add a `useEffect` under wallpaper mode:

```ts
useEffect(() => {
  if (!wallpaperMode) return;

  return installVisibilityController({
    onHidden: () => setRunning(false),
    onVisible: () => {},
  });
}, [wallpaperMode]);
```

Prefer gating the hidden behavior with `wallpaperSettings.autoPause` from the existing `useWallpaperSettings()` hook.

### B. Autosave

Import:

```ts
import { useAutosave } from "@/hooks/useAutosave";
```

Then:

```ts
useAutosave(simRef.current, wallpaperMode, 60_000);
```

### C. Quality profiles

The repository already exposes a `quality` setting in `useWallpaperSettings()`. Use:

```ts
import { applyQualityProfile } from "@/lib/qualityProfiles";

const capped = applyQualityProfile(
  simRef.current.settings,
  wallpaperSettings.quality,
  wallpaperSettings.maxPopulation,
);

simRef.current.settings = capped;
```

Apply when quality/max-population changes, not every animation frame.

### D. Reduced motion

Import and call:

```ts
const reducedMotion = useReducedMotion();
```

Pass it to the cinematic/rendering layer and suppress nonessential effects when true.

### E. World sharing

Keep a seed:

```ts
const seedRef = useRef(randomSeed());
```

Import:

```ts
import {
  createWorldShare,
  readWorldFromUrl,
} from "@/lib/worldShare";
import { createSeededWorld, randomSeed } from "@/sim/seededWorld";
```

At initial setup, check for a shared world and prefer it.

### F. Share UI

Render:

```tsx
<WorldShareDialog
  seed={seedRef.current}
  settings={sim.settings}
  onLoadShare={(seed, nextSettings) => {
    seedRef.current = seed >>> 0;
    const next = createSeededWorld(DEFAULT_SETTINGS, {
      seed: seedRef.current,
      settings: nextSettings,
    });

    simRef.current = next;
    cinematicRef.current = new CinematicCamera(next);
    setSettings(next.settings);
    setRunning(true);
  }}
  onClose={() => setShowWorldShare(false)}
/>
```

## Verification

```bash
npm run typecheck
npm run lint
npm run test
npm run build
npm run build:wallpaper
npm run package:wallpaper
```

Then manually verify:

- close tab/window
- reopen and recover autosave
- switch low/medium/high quality
- hide wallpaper
- restore wallpaper
- open shared URL
- resize to 21:9
- resize to normal 16:9
- use reduced-motion OS setting
