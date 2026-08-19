# 🚀 Polygonal Primordials — Reproducible Release Pipeline

**Goal:** One command produces a v1 release artifact. Every step is
verified. No manual work. No fake claims.

---

## 1. RELEASE COMMANDS

```bash
# Full release build
./release/build_release.sh 1.0.0

# Dry run (no artifacts written)
./release/build_release.sh 1.0.0 --dry-run

# Specific artifact only
./release/build_release.sh 1.0.0 --only=web-app
./release/build_release.sh 1.0.0 --only=wallpaper-engine

# Skip tests (NOT recommended)
./release/build_release.sh 1.0.0 --skip-tests
```

---

## 2. SCRIPT STRUCTURE (build_release.sh)

```bash
#!/usr/bin/env bash
# build_release.sh — reproducible release for Polygonal Primordials
# Usage: ./release/build_release.sh <version> [options]

set -euo pipefail

VERSION="${1:-}"
DRY_RUN=false
ONLY=""
SKIP_TESTS=false

# Parse args...

# ─── 1. VALIDATE ENVIRONMENT ────────────────────────
require_command node
require_command npm
require_command git
validate_version "$VERSION"

# ─── 2. CLEAN ────────────────────────────────────────
clean_build_artifacts

# ─── 3. INSTALL DEPENDENCIES ────────────────────────
npm ci

# ─── 4. RUN LINT ────────────────────────────────────
npm run lint || die "Lint failed"

# ─── 5. RUN TYPE CHECK ──────────────────────────────
npm run typecheck || die "Type check failed"

# ─── 6. RUN UNIT TESTS ──────────────────────────────
if [ "$SKIP_TESTS" = false ]; then
    npm test || die "Tests failed"
fi

# ─── 7. BUILD WEB APP ───────────────────────────────
npm run build || die "Web app build failed"

# ─── 8. PACKAGE WALLPAPER ENGINE ─────────────────────
build_wallpaper_engine_package || die "Wallpaper package failed"

# ─── 9. GENERATE CHECKSUMS ──────────────────────────
generate_checksums

# ─── 10. COPY TO RELEASE DIRECTORY ───────────────────
copy_artifacts_to_release_dir

# ─── 11. GENERATE RELEASE NOTES ──────────────────────
generate_release_notes "$VERSION"

# ─── 12. REPORT ──────────────────────────────────────
print_release_summary

echo "✅ Release $VERSION built successfully"
echo "📁 Artifacts in: ./releases/$VERSION/"
ls -la "./releases/$VERSION/"
```

---

## 3. RELEASE ARTIFACTS

```text
releases/v1.0.0/
├── web-app/
│   ├── primordials-web-1.0.0.zip       # Built web app
│   └── README.md                        # What's in the zip
│
├── wallpaper-engine/
│   ├── workshop-preview.mp4             # 30s preview
│   ├── workshop-preview.gif             # GIF fallback
│   ├── workshop-thumbnail.png           # 1920×1080
│   ├── workshop-description.md          # Description text
│   └── README.md                        # Submit instructions
│
├── itch.io/
│   ├── primordials-itch-1.0.0.zip       # itch-ready
│   ├── cover.png                        # 1280×720
│   ├── screenshots/                     # 5+ screenshots
│   └── README.md                        # Upload steps
│
├── checksums/
│   └── SHA256SUMS.txt
│
├── metadata/
│   ├── version.json
│   ├── build-info.json
│   ├── commit-sha.txt
│   └── release-notes.md
│
└── README.md                            # What to do with all this
```

---

## 4. PRE-RELEASE VALIDATION CHECKLIST

The script MUST verify before building:

- ✅ Git working directory clean (no uncommitted changes)
- ✅ On main branch (or release branch)
- ✅ Version not already released
- ✅ All tests pass
- ✅ Lint passes
- ✅ Type check passes
- ✅ Build succeeds
- ✅ CHANGELOG.md has entry for this version
- ✅ LICENSE file exists and is valid
- ✅ README is up to date

---

## 5. POST-RELEASE CHECKLIST

After script completes, manually verify:

- [ ] Run web app locally → does it start?
- [ ] Open default biome → does it simulate?
- [ ] Switch biomes → all 6 work?
- [ ] Pause / resume → works?
- [ ] Speed controls → work?
- [ ] Open on different screen → responsive?
- [ ] Open DevTools → no errors?
- [ ] Check FPS → meets target?
- [ ] Open on real hardware → works?
- [ ] Test offline → works?
- [ ] Test wallpaper mode URL → works?

*Only after ALL manual checks pass → proceed to distribution.*

---

## 6. VERSION TAGGING

After successful release:

```bash
git tag -s v1.0.0 -m "Release v1.0.0"
git push origin v1.0.0
```

This creates the GitHub release.
The CI workflow (`release.yml`) auto-builds + uploads the artifacts.

---

## 7. RELEASE NOTES TEMPLATE

```markdown
# Polygonal Primordials v1.0.0 — First Public Release

**Release date:** [DATE]
**Status:** Stable
**Platforms:** Web, Wallpaper Engine, itch.io

## What's new

This is the first public release of Polygonal Primordials. Six biomes
included, full wallpaper support, interactive simulation.

## Features

- 🧬 Real-time predator-prey ecosystem simulation
- 🌿 6 default biomes (algae bloom, deep sea, primordial soup,
       tundra, coral reef, volcanic vent)
- 🖥️ Live wallpaper mode
- 🎮 Interactive controls (pause, speed, zoom, pan)
- ⚙️ Settings (entity count, performance mode)
- 💾 Local save / load

## Verified performance

Tested on:
- Windows 10/11, modern browsers (Chrome, Firefox, Edge)
- 60 FPS at [X] entities on [spec hardware]

## What's NOT in this release

- ❌ Multiplayer (planned for v2.x)
- ❌ Steam release (planned after market validation)
- ❌ Mobile (planned for v3.x)
- ❌ Native C++ standalone (planned for v4.x)

## Known issues

- [list known issues from GitHub]
```

---

## 8. WHAT THIS REPLACES

The current repository has:
- `build_all.sh`, `build_all.bat` — partial automation
- `create_release.sh`, `create_release.bat` — partial automation
- `release/` directory — existing assets
- `packaging/` directory — Windows installer scripts
- `installer/` directory — Inno Setup scripts

This consolidated `build_release.sh` becomes the single source of truth. Old scripts become wrappers or are removed.
