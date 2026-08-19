# Phase 3 — Apply Instructions

Phase 3 assumes Phase 1 and Phase 2 are already copied.

## 1. Copy files

Merge the new `src/`, `tests/`, `tools/`, and `docs/phase3/` files.

## 2. Runtime watchdog

In the component that owns the existing RAF loop (`SimCanvas.tsx`), import:

```ts
import { RuntimeWatchdog } from "@/lib/runtimeWatchdog";
```

Create one instance with `useRef()`.

Inside the existing loop:

```ts
const event = watchdogRef.current.frame(frameMs, steps > 0);
```

## 3. Telemetry

Import:

```ts
import { RuntimeTelemetryCollector } from "@/lib/runtimeTelemetry";
```

Use one ref.

At the end of the render/update cycle, record performance and simulation data.

## 4. Adaptive quality

Only enable it for wallpaper mode.

Use `AdaptivePerformanceController` with the existing `useWallpaperSettings()` quality value.

Do not enable it during benchmark capture.

## 5. Crash recovery

In `App.tsx` after the simulation is created:

```ts
const recovery = useRuntimeRecovery(simRef.current);
```

Show `RuntimeRecoveryBanner` if `recovery.uncleanPreviousRun` is true.

When restoring:
- replace `simRef.current`
- recreate `CinematicCamera`
- update React settings state
- restart simulation

## 6. Display metrics

Use `useDisplayMetrics()` for:
- camera bounds
- safe UI placement
- rendering scale decisions

Do not mutate simulation world dimensions when a monitor is resized.

## 7. Runtime Health HUD

Development only:

```tsx
<RuntimeHealthHud
  sample={performanceSample}
  adaptive={adaptiveRef.current.getState()}
  watchdogUnhealthy={watchdogRef.current.isUnhealthy()}
  visible={import.meta.env.DEV}
/>
```

## 8. Package scripts

Add:

```json
{
  "scripts": {
    "soak": "vitest run"
  }
}
```

The included `tools/run-soak-test.mjs` is intentionally conservative and fails if the project's test environment has not been wired.

## 9. Verification

```bash
npm run typecheck
npm run lint
npm run test
npm run build
npm run build:wallpaper
npm run package:wallpaper
```

## 10. Manual QA

Run the desktop wallpaper:

1. Start at Low.
2. Run 1 hour.
3. Repeat at Medium.
4. Repeat at High.
5. Resize window.
6. Change resolution.
7. Hide/show.
8. Sleep/wake.
9. Force-close.
10. Restart.
11. Verify recovery.
12. Leave High quality running overnight.

For a final release candidate, complete 24h and 72h runs.
