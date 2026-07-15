# Changelog

## [Unreleased] — v2 cleanup

### Removed
- `LiveOps/` module (dashboard, anomaly, rollback, A/B testing)
- `Bots/Discord/` source
- Phase H (Live Operations) section from README — was describing deleted code
- `test_ecs.exe` and `build_fix.patch` from repo root (no provenance, binaries/patches don't belong in git)

### Changed
- README "Live Operations" section rewritten as "Planned (v2.x)" — the deleted code is now in the roadmap, not the present tense

## [1.0.0] - Phase G: Release
- Finalized Polygonal Primordials standalone game build structure
- Added Steamworks integration (Achievements, Workshop, Cloud Saves)
- Added telemetry and crash reporting
- Created release tools (Asset Cooker, Docs, Trailer, Localization)
- Built deployment pipeline via GitHub Actions
