# Phase 5 — Visual Presentation

## Current repository integration points

The live repository already has:
- theme definitions in `src/sim/themes.ts`;
- `WallpaperTheme` with background/creature palette/vignette properties;
- a dedicated wallpaper mode;
- `WallpaperDock`;
- `CinematicCamera`.

Phase 5 adds presentation layers around these existing systems rather than changing simulation rules.

## 1. Import CSS

From your existing global entry point:

```ts
import "@/styles/phase5.css";
```

## 2. Presentation mode

In `App.tsx`:

```ts
import { usePresentationMode } from "@/hooks/usePresentationMode";
const presentation = usePresentationMode();
```

## 3. Visual overlay

```tsx
<VisualEffectsOverlay
  theme={theme}
  cinematic={presentation.isCinematic}
  captureMode={presentation.isCapture}
  reducedMotion={reducedMotion}
/>
```

Place it after `SimCanvas` and before interactive controls.

## 4. Capture controls

```tsx
<PresentationChrome
  mode={presentation.mode}
  onModeChange={presentation.setMode}
  onClose={() => presentation.setMode("normal")}
/>
```

## 5. Capture frame

```tsx
<CaptureFrame
  sim={simRef.current}
  theme={theme}
  visible={presentation.isCapture}
/>
```

## Rules

Capture mode is strictly for screenshots/trailers/press media. Never make it a hidden gameplay requirement.

Retail build:
- no debug HUD;
- no release preflight;
- no development version badge;
- no fake statistics;
- real simulation only.

## Loading screen

Display the loading component until the actual simulation/camera/theme initialization is complete.
