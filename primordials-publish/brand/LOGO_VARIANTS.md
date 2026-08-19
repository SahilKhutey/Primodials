# 🎯 PRIMODIALS — Logo Variants & Usage

## THE SELECTED MARK (Recommended: VARIANT 01)

```
      ┌─────┐
     ╱       ╲
    │  ●──▶   │      Legend:
     ╲       ╱       ● = Prey (circle, bone white)
      └─────┘        ▶ = Predator (triangle, magenta)
```

A perfect-circle prey cell with a triangle predator on a thin orbital path. Encodes the predator-prey Lotka-Volterra dynamic visually.

### Why this wins:
- ✅ Instantly readable as "ecosystem"
- ✅ Scales perfectly from 16px to 4K
- ✅ Works in mono (single color)
- ✅ Distinctive — not a generic dot or triangle
- ✅ Tells the story without words

---

## LOGO VARIANTS — REQUIRED SET

### V1. Primary Mark (Color, Square)
- **File:** `assets/logo/logo-primary.svg`
- **Background:** Deep teal `#0F3D3E`
- **Prey:** Bone white `#F4F1DE`
- **Predator:** Bioluminescent magenta `#FF4FA3`
- **Orbital path:** 1.5px stroke, white at 30% opacity
- **Padding:** 12% on all sides

### V2. Mono Light (On Dark)
- **File:** `assets/logo/logo-mono-light.svg`
- **All elements:** Bone white `#F4F1DE`
- **Background:** Transparent (or deep teal)
- **Use:** Dark mode, marketing dark surfaces

### V3. Mono Dark (On Light)
- **File:** `assets/logo/logo-mono-dark.svg`
- **All elements:** Void black `#0A0908`
- **Background:** Transparent (or bone white)
- **Use:** Light mode, print

### V4. Icon Only (No Wordmark)
- **File:** `assets/logo/logo-icon.svg`
- **Design:** Just the organism mark in a 1:1 square
- **Use:** Favicon, app icon, social avatar

### V5. Horizontal Lockup (Mark + Wordmark)
- **File:** `assets/logo/logo-horizontal.svg`
- **Layout:** Mark on left, `primordials.` wordmark on right
- **Aspect ratio:** ~4:1
- **Use:** Website header, GitHub README top

### V6. Stacked Lockup (Mark Above Wordmark)
- **File:** `assets/logo/logo-stacked.svg`
- **Layout:** Mark on top, `primordials.` below
- **Aspect ratio:** 1:1.4
- **Use:** Square spaces, GitHub social preview

### V7. Inverse Color (Light Background)
- **File:** `assets/logo/logo-inverse.svg`
- **Background:** Bone white
- **Mark colors:** Deep teal + magenta
- **Use:** Light backgrounds

### V8. Animated (Optional)
- **File:** `assets/logo/logo-animated.svg`
- **Behavior:** SVG with CSS animation; triangle orbits the circle slowly (8s loop)
- **Use:** Website hero

---

## LOGO USAGE RULES

### ✅ DO
- Maintain minimum clear space = 1× the height of the prey circle.
- Use the full lockup (mark + wordmark) for first-time audiences.
- Use icon-only for favicon, app icons, and social avatars.
- Pair with bone white text on dark backgrounds.

### ❌ DON'T
- Don't rotate or skew the mark.
- Don't recolor with off-brand colors.
- Don't add drop shadows or 3D effects.
- Don't place on busy photographic backgrounds without an overlay.
- Don't stretch the proportions.

---

## SIZE GUIDELINES

| Context | Min Size | Recommended |
| :--- | :--- | :--- |
| **Favicon** | 16×16 px | 32×32 px |
| **App Icon** | 64×64 px | 512×512 px |
| **Avatar** | 32×32 px | 256×256 px |
| **Header (Web)** | 32px Height | 48–64px Height |
| **Print** | 8mm | 15–25mm |
| **App Store Capsule** | 512×512 px | 1024×1024 px |

---

## TECHNICAL SPECS
- **Vector format:** SVG 1.1 (no external fonts required)
- **PNG fallback:** PNG-24 with alpha
- **Color space:** sRGB
- **Stroke widths:** Fixed at 1.5px or 2.5px
- **Snap to pixel grid** at small sizes
