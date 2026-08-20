# Phase 6 — Release Candidate Pipeline

The current package.json already defines:
- `build`
- `build:wallpaper`
- `package:wallpaper`
- `typecheck`
- `lint`

Phase 6 adds explicit release evidence around those commands.

## Required sequence

```bash
npm ci
npm run typecheck
npm run lint
npm run test
npm run build
npm run build:wallpaper
npm run package:wallpaper
node tools/validate-release.mjs
node tools/create-build-info.mjs
node tools/generate-release-manifest.mjs
```

`npm run test` comes from the Phase 1 Vitest implementation.

## Artifact discipline

Place final distributables under:

```text
release/artifacts/
```

Then generate SHA-256 records with:

```bash
node tools/generate-release-manifest.mjs
```

## Version

The current repository package version is `1.0.0`. Keep `src/lib/version.ts` synchronized with it.

Before tagging:

```bash
git status --short
```

Then only after all critical checklist items pass:

```bash
git tag -a v1.0.0-rc.1 -m "Polygonal Primordials 1.0.0 RC1"
git push origin v1.0.0-rc.1
```

## Store truth

Every marketing claim must originate from the exact release candidate:
source → build → real test → evidence → store copy.
