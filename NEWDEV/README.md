# Deployment Instructions

This package contains every fix from this session, consolidated into one patch instead of
scattered across a long conversation.

## Contents
- `all-changes.patch` — every modification to existing tracked files (14 files: App.tsx,
  simulation.ts, genetics.ts, brain.ts, renderer.ts, chemicalField.ts, supabase.ts,
  HistoryPanel.tsx, InspectorPanel.tsx, package.json, plus 3 C++ files)
- `new-files/` — 5 brand-new files that need to be added, not patched:
  - `.env.example` → repo root
  - `.env.wallpaper` → repo root
  - `WallpaperInfoPopover.tsx` → `src/components/`
  - `wallpaperEngineBridge.ts` → `src/lib/`
  - `spatialGrid.ts` → `src/sim/`

## Steps

1. **Back up or branch first** — this touches 14 files:
   ```bash
   git checkout -b session-fixes
   ```

2. **Apply the patch** from your repo root:
   ```bash
   git apply --check all-changes.patch   # dry run, reports conflicts without touching anything
   git apply all-changes.patch           # applies for real if the dry run was clean
   ```
   If it doesn't apply cleanly (likely only if you've made your own edits to these files since
   this session started), tell me which file failed and I'll give you that file's exact
   before/after content directly instead.

3. **Add the 5 new files** — copy everything from `new-files/` into the paths listed above.

4. **Verify:**
   ```bash
   npm install
   npm run typecheck
   npm run build
   npm run build:wallpaper
   ```
   All four should complete clean, matching what was verified throughout this session.

5. **Commit:**
   ```bash
   git add -A
   git commit -m "Fix Supabase crash, wallpaper mode bugs, neural/genetics correctness fixes, spatial index, Ecosystem Mood"
   git push origin session-fixes
   ```
   (or merge/push to `main` directly, your call)

## What you'll have after this

- The app boots without crashing on a fresh clone (the original bug that started this session)
- A dedicated `npm run build:wallpaper` producing a Workshop-ready build with no editor UI
- 19 of 25 tracked correctness/feature fixes from `test-plan.md`, all empirically verified
- Everything documented in `test-plan.md`, `development-checklist.md`,
  `production-development-plan.md`, and `repo-state-reanalysis.md` (shared earlier this session)
  will then actually describe your real repository, not just this sandbox.
