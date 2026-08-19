# ⚖️ Polygonal Primordials — License Decision

**Status:** DECISION REQUIRED BEFORE PUBLIC LAUNCH
**Authority:** Sahil Khutey (Founder)

---

## 1. CURRENT STATE (the problem)

The repository currently contains contradictory license information:

| Document | Claim |
|----------|-------|
| README.md | "License: MIT" |
| Marketing materials | "MIT licensed" |
| Website copy | "MIT licensed, open source" |
| `LICENSE` file | **"PROPRIETARY AND CONFIDENTIAL"** |

This contradiction makes the project legally ambiguous and damages
publisher credibility.

**The license file wins legally.** If the LICENSE file says
proprietary, all the README's MIT claims are marketing, not law.

---

## 2. THE TWO REAL OPTIONS

### Option A — Genuinely MIT (open source)

Keep source code open under MIT.

**Pros:**
- Community can fork, contribute, learn
- Aligns with current README/marketing claims
- Easier to get contributors
- "Open source" as marketing message is real

**Cons:**
- Anyone can redistribute the product for free
- Cannot sell exclusive features
- Anyone can repackage under their own Steam page
- Revenue relies on donations / extras

### Option B — Genuinely Proprietary

Make the LICENSE file accurate: proprietary.

**Pros:**
- Full commercial control
- Can sell exclusive features
- Can license / sub-license
- Protects against repackaging

**Cons:**
- "Open source" marketing must be removed
- Smaller contributor pool (no one can fork freely)
- Community may feel misled if we previously said MIT

---

## 3. RECOMMENDED DECISION (for this publisher)

**Decision: HYBRID**

| Layer | License | Reasoning |
|-------|---------|-----------|
| `src/` (web app) | MIT | Free distribution, Wallpaper Engine |
| `Engine/` (C++ engine) | MIT | Engine becomes reusable library |
| `Games/PolygonalPrimordials/` | Proprietary | Commercial game IP protected |
| `biomes/*.json` | CC0 | User-contributed, maximum sharing |
| `branding/`, `marketing/` | Proprietary | Brand assets protected |
| `docs/`, build scripts | MIT | Documentation free |

**Rationale:**
- Wallpaper Engine product (`src/`) is MIT — encourages adoption
- Engine (`Engine/`) is MIT — becomes reusable library
- The "game" (`Games/`) is proprietary — commercial IP
- User biomes are CC0 — community ownership
- Brand assets are proprietary — protect identity

---

## 4. FILES TO UPDATE (after decision)

### To make LICENSE file accurate (Option B or hybrid)

**Remove from README:**
```markdown
[![MIT License](...)](LICENSE)
# License: MIT
MIT licensed.
```

**Remove from marketing:**
- MIT licensed
- Open source

**Update LICENSE file:**

Replace current "PROPRIETARY AND CONFIDENTIAL" with one of:

If full MIT:
```text
MIT License

Copyright (c) 2026 Sahil Khutey (Gensouls Lab)

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software... [standard MIT text]
```

If hybrid (recommended):
```text
LICENSE files in this repository:

src/           → MIT (see src/LICENSE)
Engine/        → MIT (see Engine/LICENSE)
biomes/        → CC0  (see biomes/LICENSE)
Games/...      → Proprietary (see Games/LICENSE)
branding/      → Proprietary (all rights reserved)

All other directories: see individual LICENSE files.
```

---

## 5. IMMEDIATE FIX (before any public promotion)

Replace the current LICENSE file with a temporary placeholder:

```text
# LICENSE — DECISION PENDING

This repository's license structure is being finalized.
Current contents are © 2026 Sahil Khutey (Gensouls Lab).

For inquiries: license@primordials.dev
```

This avoids any legal ambiguity until final structure is decided and individual LICENSE files are placed in each subdirectory.

---

## 6. TIMELINE

- **Week 1:** Decide hybrid model (above)
- **Week 2:** Update LICENSE files
- **Week 3:** Update README + marketing
- **Week 4:** Public launch with accurate licensing

---

## 7. LEGAL REVIEW

Before public commercial launch:
- Have a lawyer review final LICENSE structure
- Have a lawyer review Privacy Policy
- Have a lawyer review EULA for any paid product
- Register trademark if pursuing brand protection
- Document all third-party licenses

This decision should be made before any further marketing.
