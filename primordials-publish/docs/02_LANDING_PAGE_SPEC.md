# 🌐 PRIMODIALS — Landing Page Architecture & UI/UX Spec

**Domain:** `primordials.dev` / `primordials.gensoulslab.dev`  
**Framework:** Next.js 14 (App Router) + Tailwind CSS + Framer Motion  
**Deployment Target:** Vercel (Edge Network)  

---

## 1. PAGE COMPONENT HIERARCHY

```
LandingPage/
├── Navbar (Glassmorphism, Live GitHub Star Badge, Quick Download Button)
├── HeroSection (Live Interactive Canvas Background + Headline + CTAs)
├── LiveStatsRibbon (60 FPS Benchmark, 250K Entities, 0% Data Collection)
├── BiomeShowcase (Interactive Carousel: Deep Sea, Aurora, Coral Reef, etc.)
├── FeatureMatrix (Real-time Ecology, Neural Brains, Low CPU, Offline First)
├── DualEditionCompare (Wallpaper Edition vs. Laboratory Engine Edition)
├── CommunityAndReviews (Discord link, Reddit mentions, Open Source GitHub link)
└── Footer (License, Creator link, Socials, Press Kit)
```

---

## 2. SECTION DESIGN SPECS

### 2.1 Hero Section
* **Visual Anchor:** An embedded, interactive mini-simulation canvas rendering smooth drifting organisms in the Abyssal Teal / Bioluminescent Magenta palette.
* **Primary Headline:**
  > **"A living ecosystem that breathes on your desktop."**
* **Sub-headline:**
  > *"Watch real Lotka-Volterra predator-prey dynamics unfold in real time. Runs as a lightweight wallpaper, screensaver, or interactive sandbox."*
* **CTAs:**
  * Primary Button: `[ ⬇️ Download Free for Desktop ]` (Auto-detects OS: Windows / macOS / Linux)
  * Secondary Button: `[ 🎮 View on Wallpaper Engine ]`
  * Secondary Outline: `[ ⭐️ Star on GitHub ]`

### 2.2 Biome Carousel
* Six visual cards with real rendering snapshots:
  1. **Deep Sea:** Bioluminescent cephalopod-inspired entities in abyssal waters.
  2. **Coral Reef:** Vibrant turquoise and coral organisms with high population density.
  3. **Aurora Borealis:** Ethereal translucent entities dancing beneath shifting auroras.
  4. **Microorganisms:** High-speed bacterial chemotaxis and nutrient colony clustering.
  5. **Forest Floor:** Slow-growing fungi, foragers, and cyclical spore bursts.
  6. **Crystal Cave:** Geometric, mineral-consuming lifeforms with radiant light reflections.

### 2.3 Interactive "Try in Browser" Feature
* Instant 1-click modal or inline canvas where visitors can inject food particles, spawn predators, and manipulate carrying capacity before downloading the full wallpaper.

---

## 3. ASSET & PERFORMANCE TARGETS
- **Core Web Vitals:** Lighthouse score 98+ across Performance, Accessibility, Best Practices, SEO.
- **Image Optimization:** Next.js `<Image />` with WebP/AVIF format.
- **Dark Mode Default:** OLED-friendly backgrounds (`#0A0908` to `#0F3D3E`).
