# Polygonal Primordials 1.0.0 RC1 — Release Notes

## Release Information
- **Product**: Polygonal Primordials
- **Package Version**: `1.0.0`
- **Release Channel**: `release-candidate`
- **Release Date**: 2026-08-21

---

## What's New in 1.0.0 RC1

### 🎨 Phase 5 — Visual Presentation & Polish
- **Presentation Modes**: Interactive mode switcher supporting `normal`, `cinematic`, and `capture` viewports with URL parameter synchronization (`?presentation=...`).
- **Visual Effects Overlay**: Atmospheric radial gradients dynamically tinted by ecosystem theme palettes, subtle vignette framing, and drifting grid lines.
- **Capture Frame**: Lower-third telemetry overlay for clean screenshots, trailer recording, and streaming showing live generation, population, and species metrics.
- **Ambient Loading Screen & Version Badge**: Initial asset loader with smooth fade-in and development mode version badges.
- **Reduced Motion Support**: Accessible `@media (prefers-reduced-motion)` transitions across all visual overlays and HUD elements.

### 🚀 Phase 6 — Release Candidate Pipeline & Store Truth
- **Automated 14-Stage Release Gate** (`npm run release:gate`): Single-command pipeline executing typechecking, linting, test matrix execution, performance benchmarks, web compilation, Wallpaper Engine bundling, publishing preflight, store manifest generation, and artifact integrity auditing.
- **Machine-Readable Metadata**: Automatic generation of `release/build-info.json`, `release/preflight.json`, `release/publish-preflight.json`, `release/store-manifest.json`, and SHA-256 `release/manifest.json`.
- **Packaging Automation**: Production web bundle (`dist/`), dedicated wallpaper bundle (`dist-wallpaper/`), and ready-to-publish Wallpaper Engine Workshop artifact (`dist-wallpaper-engine/`).

### 📦 Phase 7 — Publishing & Distribution Architecture
- **Multi-Tier Release Channels**: Runtime environment support for `development`, `qa`, `beta`, and `stable` with top-left non-intrusive HUD channel badge (`ReleaseChannelBadge`).
- **Local-First Telemetry**: Offline-first session and milestone event tracking (`launch`, `onboarding`, `wallpaper-start`, `share-world`, `local-save`, `recovery`) with zero third-party network requests.
- **itch.io Butler Integration**: Channel-based differential deployment script (`Tools/publish-itch.mjs` & `Scripts/publish-itch.ps1`) supporting `windows-beta`, `windows-stable`, `linux-beta`, and `macos-beta`.
- **Steam Store Blueprint & Checklist**: Complete Steamworks checklist, coming-soon pipeline, and strict store truth verification rules in `release/steam/`.
- **Wallpaper Engine Acceptance Checklist**: 17-point runtime and workshop acceptance checklist in `release/wallpaper-engine/`.

### 🧪 Advanced QA Architecture (Layers 1–10)
- **Deterministic Test Matrix** (40 test files, 96 tests — 100% PASS):
  - **Unit Layer** (10 files, 30 tests): Deterministic PRNG sequences, settings bounds clamping, safe storage with quota fallback, `PP1.` world sharing protocol, quality profiles, performance telemetry, watchdog hysteresis, autosave snapshots, theme palettes.
  - **Simulation Invariants Layer** (5 files, 9 tests): Finite math invariants ($x, y, v_x, v_y, \text{energy} \neq \text{NaN}$), seed determinism, event systems (meteor strikes, food seeding), advanced biology subsystem coexistence.
  - **Integration Layer** (7 files, 18 tests): Deep snapshot restoral with Float32Array neural brain weights, App/SimCanvas runtime loop contracts, package integrity, end-to-end full smoke flow.
  - **Fault Injection Layer** (4 files, 14 tests): DEV-only fault simulation (`?fault=...` / `VITE_PP_FAULT`) for QuotaExceeded errors, malformed URL payloads, corrupt JSON snapshots, and crash recovery.
  - **Compatibility & Migration Layer** (4 files, 4 tests): Schema migration helpers preserving user worlds across engine updates and canonical `PP1.` payload decode validation.
  - **Publishing & Distribution Layer** (2 files, 4 tests): Release channel verification and Steam store template / Wallpaper Engine checklist contract tests.
  - **Release Metadata Layer** (2 files, 5 tests): Release tooling and identity validation.
- **Browser End-to-End Suite** (`npm run test:e2e`): 13 Playwright Chromium specifications covering startup, first-run onboarding, keyboard hotkeys, settings persistence, world sharing, unclean exit recovery, and ultrawide viewports.
- **Hardware Profiler & Benchmarks**: Real hardware profiler (`Scripts/hardware-report.ps1`) and browser frame metrics recorder (`Tools/run-browser-benchmark.mjs`).

---

## Verification Summary (Measured on RC1 Build)

| Gate | Status | Measured Result |
|---|---|---|
| **Typecheck** | **PASS** | 0 TypeScript compile errors (`tsc --noEmit`) |
| **Lint** | **PASS** | 0 ESLint errors (`eslint .`) |
| **Test Matrix** | **PASS** | 40 test files passed (96/96 tests) |
| **Browser E2E** | **PASS** | 13/13 Playwright Chromium tests passed |
| **Performance Benchmark** | **PASS** | 60.0 FPS, 0.13ms avg frame time, 5MB heap |
| **Web Build** | **PASS** | Production bundle generated in `dist/` |
| **Wallpaper Build** | **PASS** | Standalone wallpaper bundle in `dist-wallpaper/` |
| **Wallpaper Package** | **PASS** | Workshop artifact generated in `dist-wallpaper-engine/` |
| **Wallpaper QA** | **PASS** | `project.json`, `index.html`, and `preview.png` validated |
| **Release Preflight** | **PASS** | 0 structural or package failures |
| **Artifact Audit** | **PASS** | All distribution bundles and checksums verified |
| **Publish Preflight** | **PASS** | All required commercial release documents and checklists present |
| **Store Manifest** | **PASS** | `release/store-manifest.json` generated |
| **Release Manifest** | **PASS** | SHA-256 package verification manifest updated |
