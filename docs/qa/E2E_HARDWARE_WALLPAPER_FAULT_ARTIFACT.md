# E2E + Hardware + Wallpaper Engine + Fault Injection + Artifact Validation

## A. Browser E2E

Install:
```bash
npm install -D @playwright/test
npx playwright install chromium
```

Playwright supports `baseURL`, browser projects, traces, screenshots, video and CI retries. citeturn221678search0turn221678search2turn221678search4

Add package scripts:
```json
{
  "test:e2e": "playwright test",
  "test:e2e:headed": "playwright test --headed",
  "test:e2e:debug": "playwright test --debug",
  "benchmark:browser": "node tools/run-browser-benchmark.mjs"
}
```

Run:
```bash
npm run test:e2e
```

## B. Real hardware

On a real Windows machine:
```powershell
powershell -ExecutionPolicy Bypass -File scripts/hardware-report.ps1
```

Then:
```bash
npm run benchmark:browser
```

For a 10-minute benchmark:
```text
PP_BENCHMARK_SECONDS=600
```

Run at:
- 1920×1080;
- 2560×1440;
- 3840×2160;
- 3440×1440.

Run Low / Medium / High.

Record GPU driver and hardware report with every result.

## C. Wallpaper Engine acceptance

This is a physical acceptance test, not a Vitest assertion.

For the exact release package:
1. Import/install.
2. Launch.
3. Verify initial render.
4. Verify pause/resume.
5. Verify settings.
6. Verify quality.
7. Verify auto-pause.
8. Verify fullscreen game behavior.
9. Verify sleep/wake.
10. Verify monitor resolution changes.
11. Verify dual monitor.
12. Verify 21:9.
13. Run overnight.

Wallpaper Engine's official support materials highlight crashes, black screens and installation/subscription failures among common problems, so these are explicit release gates. citeturn221678search1turn221678search7

## D. Fault injection

Development only.

Supported names:
- storage
- snapshot
- world-share
- simulation-step
- render
- wallpaper-bridge

Examples:
```text
?fault=snapshot
?fault=render
VITE_PP_FAULT=storage
VITE_PP_FAULT=wallpaper-bridge
```

Retail builds must always report:
```text
activeFault() === null
```

Fault test procedure:
1. Inject.
2. Reproduce.
3. Verify expected fallback/error path.
4. Verify no save corruption.
5. Reload.
6. Verify recovery.
7. Disable injection.
8. Re-test normal startup.

## E. Artifact validation

After release build:
```bash
node tools/validate-release.mjs
node tools/create-build-info.mjs
node tools/generate-release-manifest.mjs
```

For Windows:
```powershell
powershell -ExecutionPolicy Bypass -File scripts/windows-release-artifact-check.ps1 -ArtifactPath "release\artifacts\PolygonalPrimordials-Setup.exe"
```

## F. Evidence hierarchy

Every external result should include:
- version;
- git commit;
- date;
- machine;
- resolution;
- quality;
- test name;
- PASS/FAIL;
- screenshot/log/JSON evidence.

## G. Final gate

Automated green:
- unit/simulation/integration;
- E2E;
- production build;
- wallpaper build;
- package;
- release tooling.

Physical green:
- Windows clean install;
- Wallpaper Engine;
- hardware matrix;
- 24/72-hour soak.

Only then label the release candidate publishable.
