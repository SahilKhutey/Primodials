# Phase 2 — Productization Implementation

Phase 2 builds on the Phase 1 hardening layer and focuses on the actual desktop-wallpaper product.

## Scope

1. Quality profiles.
2. Visibility-aware auto-pause.
3. Autosave.
4. Reduced-motion awareness.
5. Responsive viewport classification.
6. Shareable world codes / URLs.
7. Release-readiness diagnostics.
8. Wallpaper status badge.
9. Share/load modal.
10. Tests for sharing and quality profiles.

## 1. Quality profiles

The existing repository already has `useWallpaperSettings()` with `low | medium | high` quality presets. The new `qualityProfiles.ts` provides deterministic simulation-budget mapping for those presets.

Recommended integration in `WallpaperDock.tsx`:

```ts
import { applyQualityProfile } from "@/lib/qualityProfiles";

const nextSettings = applyQualityProfile(
  sim.settings,
  wallpaperSettings.quality,
  wallpaperSettings.maxPopulation,
);
sim.settings = nextSettings;
```

Do not overwrite the user's full sandbox settings. Quality should act as a wallpaper performance ceiling.

## 2. Auto-pause when hidden

In the wallpaper root/app:

```ts
import { installVisibilityController } from "@/lib/visibilityController";

useEffect(() => {
  if (!wallpaperMode) return;

  return installVisibilityController({
    onHidden: () => {
      if (wallpaperSettings.autoPause) setRunning(false);
    },
    onVisible: () => {
      // Do not auto-resume unless the user explicitly enabled that behavior.
    },
  });
}, [wallpaperMode, wallpaperSettings.autoPause]);
```

The default policy is conservative: hidden desktop = pause, restore = remain paused.

## 3. Autosave

```ts
import { useAutosave } from "@/hooks/useAutosave";

useAutosave(
  simRef.current,
  wallpaperMode,
  60_000,
);
```

This gives the wallpaper an offline recovery point.

## 4. World sharing

Keep the simulation seed in a ref/state:

```ts
const seedRef = useRef(randomSeed());
```

Create a world:

```ts
const sim = createSeededWorld(DEFAULT_SETTINGS, {
  seed: seedRef.current,
});
```

Create a share code:

```ts
const code = createWorldShare(seedRef.current, {
  worldWidth: sim.settings.worldWidth,
  worldHeight: sim.settings.worldHeight,
  initialPopulation: sim.settings.initialPopulation,
  initialFood: sim.settings.initialFood,
  mutationRate: sim.settings.mutationRate,
  maxPopulation: sim.settings.maxPopulation,
  maxFood: sim.settings.maxFood,
  boundaryMode: sim.settings.boundaryMode,
  biomes: sim.settings.biomes,
  advancedBiology: sim.settings.advancedBiology,
  socialBehavior: sim.settings.socialBehavior,
});
```

A shared URL looks like:

```text
https://your-domain.example/?world=PP1....
```

Do not put organism state into URLs. Share only seed + safe settings.

## 5. Read a shared world on startup

```ts
const shared = readWorldFromUrl();

if (shared) {
  const sim = createSeededWorld(DEFAULT_SETTINGS, {
    seed: shared.seed,
    settings: shared.settings,
  });
}
```

The URL payload is intentionally versioned so future formats can be rejected safely.

## 6. Reduced motion

Use:

```ts
const reducedMotion = useReducedMotion();
```

Then reduce or disable:
- screen shake
- automatic camera movement
- intense glow/particle effects

when `reducedMotion === true`.

## 7. Responsive wallpaper

Use:

```ts
const viewport = useResponsiveWallpaper();
```

Recommended policies:
- `viewport.ultrawide`: widen cinematic overview framing.
- `viewport.portrait`: use overview camera and avoid edge-heavy UI.
- normal desktop: standard framing.

Do not change simulation world dimensions on every resize. Camera/rendering should adapt.

## 8. Release readiness

Run in development and capture:

```ts
console.table(evaluateReleaseReadiness(sim.settings));
```

Treat a failed item as a release warning, not as a crash.

## 9. Status badge

Place `<WallpaperStatusBadge />` in the wallpaper-only build if a non-invasive status overlay is desired.

Do not show it permanently in the public store screenshots.

## 10. Share modal

Use `<WorldShareDialog />` from an optional Share button.

Recommended placement:
- sandbox header: `Share World`
- wallpaper dock: compact `Share`

## 11. Testing

```bash
npm run test
```

Required Phase 2 coverage:
- world-share round trip
- invalid world-share rejection
- quality-profile budgets
- existing Phase 1 tests

## Definition of done

- quality settings actually alter simulation budgets
- hidden wallpaper can auto-pause
- autosave works without Supabase
- reload can recover the latest local state
- world seed can be shared as a URL
- shared world URL can be opened and reproduced
- reduced motion is respected
- ultrawide/resize behavior does not distort the world
- readiness diagnostics produce actionable results
- tests pass
