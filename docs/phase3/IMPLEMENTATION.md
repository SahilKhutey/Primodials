# Phase 3 — Runtime QA, Performance & Desktop Integration

## Goal

Turn the Phase 2 product layer into a runtime that can stay open for hours, recover after abnormal shutdowns, adapt to performance pressure, and provide measurable release evidence.

## A. Watchdog

Instantiate once:

```ts
const watchdogRef = useRef(new RuntimeWatchdog());
```

Inside the existing RAF loop:

```ts
const event = watchdogRef.current.frame(frameMs, steps > 0);

if (event?.type === "slow-frame" || event?.type === "simulation-stall") {
  telemetryRef.current.watchdog();
}
```

Do not stop the simulation automatically after the first warning. A watchdog is diagnostic first.

## B. Telemetry

Instantiate:

```ts
const telemetryRef = useRef(new RuntimeTelemetryCollector());
```

After each performance sample:

```ts
telemetryRef.current.lastPerformance(sample);
telemetryRef.current.simulation(
  steps,
  simRef.current.species.length,
  simRef.current.generation,
);
```

Expose a dev-only export:

```ts
const json = telemetryRef.current.exportJson();
```

Save it as a file for QA.

## C. Adaptive quality

Enable only in wallpaper mode.

```ts
const adaptiveRef = useRef(
  new AdaptivePerformanceController(wallpaperSettings.quality, 45),
);

const qualityChange = adaptiveRef.current.observe(sample);

if (qualityChange) {
  const next = applyQualityProfile(
    simRef.current.settings,
    qualityChange,
    wallpaperSettings.maxPopulation,
  );

  simRef.current.settings = next;
  setWallpaperSettings((current) => ({
    ...current,
    quality: qualityChange,
  }));
}
```

Do not enable adaptive mode in benchmark runs; a benchmark must represent fixed settings.

## D. Crash recovery

At app startup:

```ts
const runtimeRecovery = useRuntimeRecovery(simRef.current);
```

If `uncleanPreviousRun` is true, show:

```tsx
<RuntimeRecoveryBanner
  visible={!dismissed}
  onRecover={() => {
    const restored = runtimeRecovery.recover();
    if (!restored) return;

    simRef.current = restored;
    cinematicRef.current = new CinematicCamera(restored);
    setSettings(restored.settings);
    setRunning(true);
  }}
  onDismiss={() => setDismissed(true)}
/>
```

## E. Clean shutdown

The recovery hook marks the run clean during React cleanup. If the process crashes, is killed, or the tab dies, the flag remains unclean and the next start offers recovery.

## F. Display integration

Use:

```ts
const display = useDisplayMetrics();
```

Use display metrics for rendering/camera only.

Never rescale the simulation world itself on every resize.

## G. Soak testing

The pure test helper executes the existing `Simulation.step()` repeatedly and checks:
- no non-finite organism state
- no runaway population over the configured bound
- expected tick count
- deterministic seed setup

Short automated soak:

```bash
npm run test
```

Long manual soak:
- 1 hour
- 8 hours
- 24 hours
- 72 hours

Collect:
- FPS
- memory
- population
- species
- generation
- watchdog events
- crashes
- recovery success

## H. Release benchmark

Create a benchmark matrix:

| Profile | Resolution | Population | Duration |
|---|---|---:|---:|
| Low | 1080p | 125 | 10 min |
| Medium | 1080p | 250 | 10 min |
| High | 1440p | 500 | 10 min |
| High | 4K | 500 | 10 min |
| High | 21:9 | 500 | 10 min |

Record:
- average FPS
- 1% low if available
- average frame time
- peak memory
- GPU load
- CPU load
- crash count

Do not publish benchmark claims until these are collected on real hardware.

## I. Runtime health HUD

Show only in development:

```tsx
<RuntimeHealthHud
  sample={sample}
  adaptive={adaptiveRef.current.getState()}
  watchdogUnhealthy={watchdogRef.current.isUnhealthy()}
  visible={import.meta.env.DEV}
/>
```

## Definition of done

- runtime watchdog detects sustained stalls
- telemetry can be exported
- adaptive quality works only in allowed mode
- abnormal shutdown produces recovery prompt
- clean exit removes recovery warning
- resize/orientation does not corrupt world state
- short automated soak passes
- 1h/8h/24h/72h manual soak procedure is documented
- release benchmarks are reproducible
