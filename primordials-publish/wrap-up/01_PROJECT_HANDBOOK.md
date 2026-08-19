# 📖 PRIMORDIALS — Complete Master Project Handbook

---

## 📑 TABLE OF CONTENTS

1. [Project Vision & Core Tenets](#1-project-vision--core-tenets)
2. [Dual-Engine Technical Architecture](#2-dual-engine-technical-architecture)
3. [Repository Map & Structure](#3-repository-map--structure)
4. [Build, Packaging & Deployment Guide](#4-build-packaging--deployment-guide)
5. [Launch Runbook & Marketing Schedule](#5-launch-runbook--marketing-schedule)
6. [Operational Cadence](#6-operational-cadence)
7. [Community Infrastructure & Recognition](#7-community-infrastructure--recognition)
8. [Monetization & Financial Modeling](#8-monetization--financial-modeling)
9. [Long-term Technology Roadmap](#9-long-term-technology-roadmap)
10. [Contact Directory & Resources](#10-contact-directory--resources)

---

## 1. PROJECT VISION & CORE TENETS

- **Patient:** Ecosystems evolve organically; simulations favor depth over hyperactive arcade mechanics.
- **Curious:** Open inspection of all species parameters, food webs, and differential derivatives.
- **Grounded:** Based on peer-reviewed Lotka-Volterra mathematical models with Runge-Kutta numerical solvers.
- **Free First:** Open source under the permissive MIT license forever.

---

## 2. DUAL-ENGINE TECHNICAL ARCHITECTURE

```
┌────────────────────────────────────────────────────────────┐
│                    Web & Desktop App                       │
│ ├─ React 18, Vite 5, Tailwind CSS, Zustand                │
│ ├─ Hybrid Renderer (WebGPU Compute + Canvas 2D Fallback)   │
│ └─ Zero-code visual biome authoring suite & live preview   │
├────────────────────────────────────────────────────────────┤
│                 Native Laboratory Edition                  │
│ ├─ C++23, SDL3 GPU backend, AVX2/NEON SIMD intrinsics      │
│ ├─ Generational archetype SoA Entity-Component-System     │
│ └─ 250,000+ to 1,000,000+ organisms @ 60 FPS               │
└────────────────────────────────────────────────────────────┘
```

---

## 3. REPOSITORY MAP & STRUCTURE

```
Primodilas/
├── .github/                      # Issue/PR templates, CI/CD workflows
├── docs/                         # Architecture, schema, roadmap & FAQ docs
├── Engine/                       # C++23 native simulation engine
├── Games/PolygonalPrimordials/   # Native SDL3 desktop game application
├── biomes/                       # Default biome JSON definitions
├── primordials-publish/          # Complete 14-phase publishing ecosystem
│   ├── brand/ & assets/          # Brand books & scalable vector SVGs
│   ├── content/                  # Trailer, dev log & media production specs
│   ├── primordials.dev/          # Production Next.js 14 marketing landing page
│   ├── storefronts/              # Steam, itch.io & Wallpaper Engine packages
│   ├── marketing/                # Launch blitz playbooks & automation tools
│   ├── community/                # Discord bot implementations & moderation
│   ├── monetization/             # Revenue models & DLC expansion roadmaps
│   ├── master/                   # Operations manuals & decision trees
│   ├── web-app/                  # Standalone React 18 / TS wallpaper web app
│   ├── engine-improvements/      # C++23 performance source code & benchmarks
│   ├── webgpu/                   # WGSL compute shaders & 1M+ entity pipeline
│   ├── biome-tools/              # Visual biome editor & validation engine
│   └── wrap-up/                  # Consolidated project handbooks & checklists
└── package.json                  # Root monorepo build scripts
```

---

## 4. BUILD, PACKAGING & DEPLOYMENT GUIDE

```bash
# 1. Typecheck the entire codebase
npm.cmd run typecheck

# 2. Package storefront release artifacts
npm.cmd run package:all

# 3. Launch local development server
cd primordials-publish/web-app && npm.cmd run dev

# 4. Build native C++ engine (Release mode)
cmake --preset release
cmake --build --preset release
```

---

## 5. LAUNCH RUNBOOK & MARKETING SCHEDULE

- **T-0 (Launch Tuesday, 9:00 AM EST):** Simultaneous deployment across Product Hunt, Hacker News Show HN, Twitter launch thread, and Reddit communities.
- **T+1 to T+7:** Continuous author comment interaction, community onboarding in Discord, and Tier-1 press outreach.
- **T+14:** First community retrospective, GitHub release patch v2.1.1, and launch metrics debrief.
