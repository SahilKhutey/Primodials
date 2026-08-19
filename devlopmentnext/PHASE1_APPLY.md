# Phase 1 — Apply Instructions

## 1. Copy overlay

Copy the directory contents into the repository root, preserving paths.

## 2. Add Vitest

```bash
npm install -D vitest
```

## 3. Add scripts to package.json

Merge:

```json
{
  "scripts": {
    "test": "vitest run",
    "test:watch": "vitest",
    "check": "npm run typecheck && npm run lint && npm run test"
  }
}
```

## 4. Integrate persistent settings

In `src/App.tsx`:

```ts
import { usePersistentSettings } from "@/hooks/usePersistentSettings";

const {
  settings,
  setSettings,
  resetSettings,
} = usePersistentSettings(DEFAULT_SETTINGS);
```

When changing a setting, validate before assignment:

```ts
const next = validateSettings({ ...settings, [key]: value }, DEFAULT_SETTINGS);
setSettings(next);
simRef.current.settings = { ...next };
```

## 5. Add local save/load

```ts
import { saveLocalSnapshot, loadLocalSnapshot } from "@/sim/localSnapshot";
import { restoreSimulationSnapshot } from "@/sim/restoreSnapshot";

const handleSaveLocal = () => {
  saveLocalSnapshot(simRef.current);
};

const handleLoadLocal = () => {
  const snapshot = loadLocalSnapshot();
  if (!snapshot) return;

  const sim = restoreSimulationSnapshot(snapshot);
  simRef.current = sim;
  setSettings(sim.settings);
  setRunning(true);
};
```

Recreate the cinematic camera after replacing the simulation:

```ts
cinematicRef.current = new CinematicCamera(sim);
```

## 6. Integrate diagnostics

At development/startup:

```ts
import { runClientDiagnostics } from "@/lib/releaseDiagnostics";

const diagnostics = runClientDiagnostics(settings);
console.table(diagnostics);
```

## 7. Performance monitoring

Instantiate one monitor inside `SimCanvas`:

```ts
const performanceRef = useRef(new PerformanceMonitor());
```

Inside the existing RAF loop:

```ts
const frameMs = performanceRef.current.beginFrame(now);
let steps = 0;

while (acc >= STEP_MS) {
  simRef.current.step();
  acc -= STEP_MS;
  steps += 1;
}

performanceRef.current.record(
  steps,
  simRef.current.population,
  frameMs,
);
```

Do not add a second RAF loop.

## 8. Final verification

```bash
npm run typecheck
npm run lint
npm run test
npm run build
npm run build:wallpaper
npm run package:wallpaper
```

Then test:

- normal browser mode
- `?wallpaper=1`
- Wallpaper Engine
- pause/resume
- reset
- local save/load
- fullscreen
- display resize
- hidden/visible tab
- 24-hour run
