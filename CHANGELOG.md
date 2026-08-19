# Changelog

All notable changes to Primordials will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [2.1.0] - 2026-08-19

### Phase 1 — Settings Persistence, Validation & Local Snapshots
- Added `usePersistentSettings` hook with local storage backing, range validation, and safe fallbacks
- Implemented `validateSettings` enforcing physical bounds and boundary mode constraints
- Added offline-first `saveLocalSnapshot`, `loadLocalSnapshot`, and `restoreSimulationSnapshot`
- Integrated `runClientDiagnostics` and `PerformanceMonitor` with live telemetry HUD

### Phase 2 — Quality Profiles, World Sharing & Accessibility
- Added `qualityProfiles` capping simulation budgets for `low`, `medium`, and `high` rendering presets
- Implemented `worldShare` enabling URL-safe base64 encoded world seeds (`?world=PP1...`)
- Added `WorldShareDialog` for interactive share generation and instant seed loading
- Implemented `visibilityController` for automatic loop pausing when hidden or covered
- Added `useReducedMotion` and `useResponsiveWallpaper` adapting camera behavior for ultrawide (21:9+) displays

### Phase 3 — Runtime Watchdogs, Soak Testing & Crash Recovery
- Implemented `RuntimeWatchdog` detecting slow frames (>=100ms) and simulation tick stalls (>=2s)
- Added `RuntimeTelemetryCollector` with ring-buffered telemetry samples and JSON export
- Added `AdaptivePerformanceController` dynamically downshifting quality under frame rate pressure
- Added `useRuntimeRecovery` and `RuntimeRecoveryBanner` restoring previous states after abnormal termination
- Implemented `soakTest` with automated deterministic headless stress tests (`npm run soak`)
- Added `RuntimeHealthHud` for real-time engine health and FPS metrics in dev mode

### Phase 4 — Product UX, First-Run Onboarding, Shortcuts & Error Containment
- Added `AppErrorBoundary` with crash isolation and one-click app reload
- Implemented `FirstRunOverlay` with 4-step onboarding journey (Watch, Inspect, Shape, Share)
- Added `Phase4UXShell` wrapping both Sandbox and Wallpaper modes with centralized controls
- Added `useKeyboardShortcuts` (`Space`/`P`, `N`/`.`, `R`, `D`, `H`/`?`, `Ctrl/Cmd+F`) with input suppression
- Added `HelpOverlay` shortcut reference guide and glassmorphism `Toast` notification system
- Restructured `SettingsPanel` into grouped categorized cards with `SettingsSection` and `SettingsRow`

---

## [2.0.0] - 2026-07-04

### Added
- Web application (`src/`) as primary consumer product
- React 18 + TypeScript + Vite + Tailwind CSS stack
- Canvas 2D high-performance rendering pipeline
- Optional Supabase cloud-save integration (100% offline-first by default)
- Wallpaper mode (`?wallpaper=1`)
- Steam Wallpaper Engine packaging scripts
- Unified build system: `build_all.sh` / `build_all.bat`
- Cross-platform native installers: Windows `.exe`, Linux `.deb`, Linux `AppImage`, macOS `.pkg`
- 221 Catch2 unit test cases, 1,311 assertions in C++ engine
- SDL3 integration and lockstep deterministic networking protocol
- Lotka-Volterra differential equation simulation core

---

## [1.0.0] - 2024-08-14

### Added
- Initial C++ ECS simulation engine
- Custom shape rendering (polygonal organisms)
- Catch2 unit test suite
- CMake build system
- SDL3 rendering backend
- Basic predator-prey dynamics
