# 📰 PRIMODIALS — Hacker News "Show HN" Package

**Timing:** Tuesday / Wednesday, 08:30 – 09:30 AM EST  
**Submission Title:** `Show HN: Primordials – Real ecosystem simulator for your desktop (250K entities @ 60 FPS, C++23, MIT)`  
**Target URL:** `https://github.com/SahilKhutey/Primodials` (or `https://primordials.dev`)  

---

## 📝 SUBMISSION TEXT / TOP COMMENT

```markdown
Hi HN,

I built Primordials — a desktop application that runs real Lotka-Volterra predator-prey equations as a live wallpaper, screensaver, or windowed application. It's been a solo passion project over the last 6 months.

### The Mathematics:
Continuous Lotka-Volterra equations:
  dx/dt = αx − βxy  (prey growth vs. predation)
  dy/dt = δxy − γy  (predator growth vs. starvation)

When combined with spatial grid hashing, nutrient gradient fields, and genetic mutations, natural emergence and periodic carrying-capacity oscillations occur without scripting.

### Engineering & Architecture:
- **Native Engine:** Written in C++23 with a custom data-oriented Entity-Component-System (SoA storage) and SDL3.
- **Throughput:** 250,000 entities locked at 60 FPS on integrated graphics.
- **Verification:** 221 Catch2 unit tests, 1,311 assertions, bit-exact deterministic replays.
- **Web Client:** TypeScript Canvas 2D client for frictionless browser evaluation.
- **Configurability:** All biomes defined via an open JSON schema.

### Why I built it:
Every screensaver or ambient desktop app I've used is either a looping MP4 that consumes battery or a static particle toy. I wanted a real digital terrarium where populations live, evolve, and crash based on actual mathematical ecology.

Source Code (MIT): https://github.com/SahilKhutey/Primodials
Live Browser Demo: https://gensoulslab.itch.io/polygonal-primordials
Site: https://primordials.dev

I would love feedback on the ECS architecture, performance optimizations, or parameter tuning. AMA!
```
