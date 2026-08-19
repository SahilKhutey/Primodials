# 📦 Polygonal Primordials — Asset Audit

**Purpose:** Establish clear ownership status for every asset
**Deadline:** Before v1 launch

---

## 1. AUDIT TEMPLATE

For each asset, determine:

```text
Asset: [name]
Location: [path]
Type: [code / asset / font / sound / image / 3D model / shader]
Source: [created / purchased / open source / unknown]
License: [MIT / CC0 / proprietary / licensed / unknown]
Commercial OK: [yes / no / conditional]
Attribution: [required text / N/A]
Action: [keep / replace / remove / license]
```

---

## 2. ASSET CATEGORIES TO AUDIT

### Code (`src/` — React/TS)
- React itself → MIT ✅
- Vite → MIT ✅
- TypeScript → Apache 2.0 ✅
- Tailwind CSS → MIT ✅
- Zustand → MIT ✅
- Framer Motion → MIT ✅
- lucide-react → ISC ✅

### Code (`Engine/` — C++)
- SDL3 → zlib ✅
- Catch2 → BSL-1.0 ✅
- Custom ECS code → TBD (per LICENSE decision)

### Code (`Games/PolygonalPrimordials/` — C++)
- Depends on Engine/ + third-party
- TBD per LICENSE decision

### Fonts
- Space Grotesk → SIL OFL 1.1 ✅ (commercial OK)
- JetBrains Mono → SIL OFL 1.1 ✅
- Verify all UI fonts used in app

### Icons
- lucide-react → ISC ✅
- Verify all custom icons in `branding/`

### Sounds
- None currently used in v1
- If added later: must be CC0, royalty-free, or owned

### Images
- brand/Logo SVG files → OWNED (by Sahil Khutey)
- biome screenshots → must be actual product screenshots
- press kit images → must be actual product images

### Shaders
- None currently used (Canvas 2D in v1)
- Future WebGPU shaders: must be authored or MIT

### 3D Models
- N/A for v1

### Fonts in UI
- Audit `src/index.css` and any embedded font files
- Confirm commercial use allowed

---

## 3. ASSET INVENTORY (template)

```text
ASSETS TO AUDIT
├── src/ [React/TS web app]
│   ├── components/
│   │   └── ui/icons [icons to audit]
│   └── assets/ [if any]
├── Engine/ [C++ engine]
│   └── vendor/ [third-party C++ deps]
├── branding/ [brand assets]
│   ├── logo/ [owned by Sahil]
│   ├── favicon/ [owned by Sahil]
│   ├── og/ [owned by Sahil]
│   └── biomes/ [if any custom renders]
├── fonts/ [if embedded]
├── screenshots/ [if any]
└── press-kit/ [if any]
```

---

## 4. THIRD-PARTY LICENSE COMPLIANCE

For each third-party asset:
1. Locate its license
2. Determine commercial use
3. Check attribution requirements
4. Add to `THIRD_PARTY_NOTICES.md`

---

## 5. RED FLAGS

Items marked **UNKNOWN** must be resolved before launch:
- Random image downloaded from internet
- Code snippet from Stack Overflow with unclear license
- Sound effect from unknown source
- Icon from unclear origin

---

## 6. CLEANUP CHECKLIST

- [ ] All assets inventoried
- [ ] All licenses confirmed
- [ ] Attribution file created
- [ ] Unknown items resolved or removed
- [ ] Custom assets documented as owned
- [ ] Third-party notices file complete
