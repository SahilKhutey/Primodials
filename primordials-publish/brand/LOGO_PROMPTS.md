# 🎨 PRIMODIALS — Logo Generation Prompts

**Tool of choice:** Ideogram 2.0 / Midjourney v6.2 / DALL-E 3 / Recraft v3  
**Run mode:** Generate 16+ variants, select best, refine 2x.  

---

## PROMPT 1 — The Primary Mark (Recommended)
> **"Minimalist geometric logo mark for a desktop simulation game called Primordials. A single circle (representing a single-celled organism) with a small triangle (representing a predator) gently orbiting it on a thin orbital path. Deep teal #0F3D3E background, bioluminescent magenta #FF4FA3 predator, bone white #F4F1DE prey. Flat vector geometry, no gradients, no shadows, perfect for app icons. Single composition centered on a square canvas. Logo design, minimalist, Swiss design, Bauhaus inspired, clean, modern, iconic."**

**Style modifiers to append:**
- `--style raw` (Midjourney)
- `--ar 1:1`
- `--stylize 50` (low stylization for cleanness)
- **Negative:** `--no text, no words, no letters, no shading, no 3d`

---

## PROMPT 2 — Wordmark + Mark (Horizontal Lockup)
> **"Logotype for 'primordials' (lowercase, period at end) in Space Grotesk Bold. The dot of the 'i' is replaced with a tiny circle containing a smaller triangle inside (organism + predator). Color: bone white #F4F1DE on deep teal #0F3D3E. Geometric, modern, minimal, app icon style."**

---

## PROMPT 3 — Polygonal Ecosystem Hero Mark
> **"Geometric polygonal art of a minimal ecosystem: 12 small triangles (predators) and 30 circles (prey) arranged in flowing clusters on a deep teal background. Negative space dominant. Flat vector, no gradients, polygonal art style, low-poly aesthetic. Logo composition, centered, square aspect ratio."**

---

## PROMPT 4 — DNA Strand Inspired
> **"Minimalist logo: a vertical helix made of geometric segments, alternating bone white circles and bioluminescent magenta triangles, like a DNA double helix made of predator and prey organisms. Deep teal background. Flat vector, geometric, modern, biotech aesthetic."**

---

## PROMPT 5 — Tidepool (Alternative)
> **"Minimalist logo of a single circle with a thin curved orbital line crossed by a tiny triangle, set inside a perfect square frame. Colors: deep teal background, magenta accent, white mark. Bauhaus, Swiss design, flat, geometric."**

---

## PROMPT 6 — Petridish (Alternative)
> **"Top-down view of a circular petri dish with abstract organism shapes inside (circles, triangles, hexagons). The dish has a thin white outline. Deep teal background, magenta and white organisms. Minimalist, scientific, flat vector."**

---

## PROMPT 7 — Wave/Loop (Alternative)
> **"A continuous loop forming a stylized letter 'P', but the loop is made of small organism shapes (circles and triangles) flowing along a path. Bone white on deep teal. Logo design, minimal, modern."**

---

## PROMPT 8 — Ecosystem Constellation (Alternative)
> **"Minimalist logo: 3 triangles and 9 circles arranged in a balanced asymmetric constellation, connected by very thin lines. Deep teal background, magenta triangles, white circles. Vector, flat, logo composition."**

---

## NEGATIVE PROMPT (Apply to all)
```
text, letters, words, typography, watermark, signature, 3d, shading, gradients, photorealistic, photograph, blurry, low quality, ugly, distorted, deformed, multiple objects, busy composition, drop shadow, glow effect
```

---

## GENERATION RUN PLAN

### Round 1: Exploration (16 variants)
- Run PROMPT 1, 2, 3, 4 with 4 variants each = 16 images
- Save all into `brand/round-1/`

### Round 2: Refinement (8 variants)
- Pick top 4 from Round 1
- Re-run with refined color/size instructions
- Save into `brand/round-2/`

### Round 3: Final Selection (1 winner)
- Pick 1 logo from Round 2
- Manually trace/vectorize in Figma or Inkscape
- Export at: 2048×2048, 512×512, 256×256, 128×128, 64×64, 32×32

---

## MANUAL VECTORIZATION STEPS (Inkscape)
1. Open winning PNG in Inkscape
2. **Trace Bitmap** → Brightness cutoff, threshold 0.45
3. Simplify paths (`Path` → `Simplify`, runs = 3)
4. Manually clean up rough edges
5. Recreate in clean vector paths (use Bezier curves)
6. Export SVG with no metadata
7. Convert to multi-size PNG via ImageMagick:
```bash
for size in 2048 1024 512 256 128 64 32; do
  convert -background none -density 1200 logo.svg -resize ${size}x${size} logo-${size}.png
done
```
