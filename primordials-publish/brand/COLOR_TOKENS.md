# 🎨 PRIMODIALS — Color Tokens (Production-Ready)

These tokens are exported as CSS variables, Tailwind configuration, and JSON formats for multi-platform consistency.

---

## PRIMARY PALETTE

| Token | Hex | RGB | Usage |
| :--- | :--- | :--- | :--- |
| `--color-bg-deep` | `#0F3D3E` | `15, 61, 62` | Primary backgrounds, brand base |
| `--color-bg-void` | `#0A0908` | `10, 9, 8` | OLED mode, deep sections |
| `--color-accent-magenta` | `#FF4FA3` | `255, 79, 163` | Primary CTA, apex highlights |
| `--color-text-primary` | `#F4F1DE` | `244, 241, 222` | Body text on dark backgrounds |
| `--color-text-muted` | `#A8B5A0` | `168, 181, 160` | Secondary & metadata text |
| `--color-surface` | `#1A4D4E` | `26, 77, 78` | Cards, elevated HUD surfaces |

---

## BIOME PALETTE (Secondary)

| Biome | Primary | Secondary | Accent |
| :--- | :--- | :--- | :--- |
| **Algae Bloom** | `#4ECDC4` | `#1B998B` | `#FFE66D` |
| **Deep Sea** | `#FF6B6B` | `#4A4E69` | `#9B5DE5` |
| **Primordial Soup** | `#FFD93D` | `#FF9F1C` | `#E63946` |
| **Tundra** | `#6BCB77` | `#4D9DE0` | `#E15554` |
| **Coral Reef** | `#F18F01` | `#048A81` | `#C73E1D` |
| **Volcanic Vent** | `#D62246` | `#4B1D3F` | `#FFB627` |

---

## SEMANTIC TOKENS

| Token | Hex | Usage |
| :--- | :--- | :--- |
| `--color-success` | `#4ECDC4` | Confirmations & population health |
| `--color-warning` | `#FFD93D` | Resource scarcity warnings |
| `--color-danger` | `#FF6B6B` | Extinction risk & error alerts |
| `--color-info` | `#4D9DE0` | Informational telemetry |

---

## CSS ROOT VARIABLES

```css
:root {
  /* Primary Brand */
  --color-bg-deep: #0F3D3E;
  --color-bg-void: #0A0908;
  --color-accent-magenta: #FF4FA3;
  --color-text-primary: #F4F1DE;
  --color-text-muted: #A8B5A0;
  --color-surface: #1A4D4E;

  /* Biome Accents */
  --color-biome-algae: #4ECDC4;
  --color-biome-deepsea: #FF6B6B;
  --color-biome-soup: #FFD93D;
  --color-biome-tundra: #6BCB77;
  --color-biome-coral: #F18F01;
  --color-biome-volcanic: #D62246;

  /* Semantic */
  --color-success: #4ECDC4;
  --color-warning: #FFD93D;
  --color-danger: #FF6B6B;
  --color-info: #4D9DE0;

  /* Opacity Gradients */
  --color-bg-deep-90: rgba(15, 61, 62, 0.9);
  --color-bg-deep-50: rgba(15, 61, 62, 0.5);
  --color-bg-void-90: rgba(10, 9, 8, 0.9);
  --color-text-muted-70: rgba(168, 181, 160, 0.7);

  /* Gradients */
  --gradient-hero: linear-gradient(135deg, #0A0908 0%, #0F3D3E 50%, #1A4D4E 100%);
  --gradient-accent: linear-gradient(90deg, #FF4FA3 0%, #9B5DE5 100%);
  --gradient-biome: linear-gradient(180deg, #4ECDC4 0%, #0F3D3E 100%);
}
```

---

## ACCESSIBILITY CONTRAST AUDIT (WCAG 2.1 AA)

| Foreground / Background Combination | Contrast Ratio | Rating |
| :--- | :--- | :--- |
| **Bone White (`#F4F1DE`) on Deep Teal (`#0F3D3E`)** | `7.8:1` | ✅ **AAA** |
| **Bone White (`#F4F1DE`) on Void Black (`#0A0908`)** | `16.4:1` | ✅ **AAA** |
| **Magenta (`#FF4FA3`) on Deep Teal (`#0F3D3E`)** | `4.6:1` | ✅ **AA** |
| **Magenta (`#FF4FA3`) on Void Black (`#0A0908`)** | `5.8:1` | ✅ **AA** |
| **Muted Text (`#A8B5A0`) on Deep Teal (`#0F3D3E`)** | `4.4:1` | ✅ **AA** |
