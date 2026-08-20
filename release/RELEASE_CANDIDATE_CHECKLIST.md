# Polygonal Primordials — Release Candidate Checklist

## Phase 5 — Visual
- [ ] Loading screen tested.
- [ ] Cinematic mode tested.
- [ ] Capture mode tested.
- [ ] Visual effects tested.
- [ ] Reduced-motion tested.
- [ ] Retail build hides diagnostics.
- [ ] Store screenshots use the real build.

## Phase 6 — Build
- [ ] npm ci
- [ ] npm run typecheck
- [ ] npm run lint
- [ ] npm run test
- [ ] npm run build
- [ ] npm run build:wallpaper
- [ ] npm run package:wallpaper
- [ ] node tools/validate-release.mjs
- [ ] node tools/create-build-info.mjs
- [ ] node tools/generate-release-manifest.mjs

## Runtime
- [ ] 1-hour soak
- [ ] 8-hour soak
- [ ] 24-hour soak
- [ ] 72-hour soak
- [ ] crash recovery
- [ ] autosave recovery
- [ ] 1080p
- [ ] 1440p
- [ ] 4K
- [ ] 21:9
- [ ] multi-monitor
- [ ] sleep/resume

## Legal
- [ ] LICENSE matches public claims.
- [ ] third-party dependencies audited.
- [ ] assets audited.
- [ ] no unsupported store claims.

Do not publish with unresolved critical items.
