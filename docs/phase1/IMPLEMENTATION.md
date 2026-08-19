# Polygonal Primordials — Phase 1 Implementation

This package is an implementation overlay for the existing `src/` product.

## What it adds

- Safe local storage access.
- Persistent `SimSettings`.
- Settings validation/clamping.
- Offline local simulation snapshots.
- Snapshot restore helpers.
- Seeded-world helpers.
- Client release diagnostics.
- Performance monitoring primitives.
- Performance HUD component.
- Unit test for settings validation.

## Integration order

1. Copy the files into the repository.
2. Add Vitest to `devDependencies`.
3. Add `test` and `check` scripts.
4. In `App.tsx`, replace `useState(DEFAULT_SETTINGS)` with `usePersistentSettings(DEFAULT_SETTINGS)`.
5. Use `validateSettings()` before assigning settings into `sim.settings`.
6. Add local snapshot save/load buttons using `captureLocalSnapshot()` and `restoreSimulationSnapshot()`.
7. Use `PerformanceMonitor` inside `SimCanvas`'s existing RAF loop instead of introducing another loop.
8. Run `npm run check`.
9. Run the browser app in normal mode.
10. Run wallpaper mode and perform a long-run test.

## Important

This overlay deliberately does not create a second simulation loop. The repository already runs simulation steps from the existing fixed-step RAF loop in `SimCanvas.tsx`. Preserve that architecture.

The overlay also does not claim that Steam, Wallpaper Engine, native C++, multiplayer, or platform installers are validated. Those remain separate release gates.
