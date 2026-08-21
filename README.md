<!-- markdownlint-configure-file { "MD013": false, "MD033": false, "MD041": false }-->
<div align="center">

<img src="primordials-publish/assets/logo/logo-primary.svg" alt="Primordials" width="160"/>

# Primordials (Polygonal Primordials)
### A living, evolving artificial life ecosystem & desktop wallpaper.

[![MIT License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![TypeScript](https://img.shields.io/badge/TypeScript-5.5-blue.svg)](https://www.typescriptlang.org/)
[![React 18](https://img.shields.io/badge/React-18.3-cyan.svg)](https://react.dev/)
[![Tests](https://img.shields.io/badge/Tests-92_passing-brightgreen.svg)](#-automated-testing--qa)
[![E2E](https://img.shields.io/badge/E2E-Playwright_13_passing-green.svg)](#-automated-testing--qa)
[![itch.io](https://img.shields.io/badge/itch.io-Play%20Online-fa5c5c.svg)](https://gensoulslab.itch.io/polygonal-primordials)
[![Discord](https://img.shields.io/badge/Discord-Join%20Community-5865F2.svg)](https://discord.gg/primordials)
[![GitHub release](https://img.shields.io/github/v/release/SahilKhutey/Primodilas)](https://github.com/SahilKhutey/Primodilas/releases)

**Real Lotka-Volterra predator-prey dynamics and neural network evolution running at 60 FPS on your desktop. Free, open-source, MIT licensed.**

[**Download**](https://github.com/SahilKhutey/Primodilas/releases) • [**Wallpaper Engine**](https://steamcommunity.com/sharedfiles/filedetails/?id=XXXXX) • [**itch.io**](https://gensoulslab.itch.io/polygonal-primordials) • [**Discord**](https://discord.gg/primordials)

</div>

---

## 🧬 What is Polygonal Primordials?

**Polygonal Primordials** is an artificial life and evolutionary biology sandbox that transforms your display into an evolving ecosystem.

Organisms inhabit a continuous physical environment governed by emergent biological rules:
- 🐛 **Herbivores & Grazers** seek food particles, cluster into colonies, and construct structures.
- 🦈 **Predators & Carnivores** hunt, coordinate in social hierarchies, and adapt through evolutionary pressure.
- 🧠 **Neural Brains** evolve connection weights across generations to navigate, avoid hazards, and cooperate.
- 🌿 **Microbial Behaviors** exhibit chemotaxis, biofilm formation, quorum sensing, and horizontal gene transfer (HGT).
- ⚖️ **Ecosystem Balance** dynamically reacts to environmental changes, disease events, and catastrophe strikes.

---

## ✨ Features

| Feature | Description |
|:---|:---|
| 🧬 **Real Evolutionary Biology** | Evolving neural networks, mutation engine, sexual reproduction, and genetic drift. |
| 🌿 **Dynamic Themes & Biomes** | Deep Sea, Coral Reef, Aurora, Microorganisms, Forest Floor, Crystal Cave, Cyberpunk, and Solar Flare. |
| 🎬 **Presentation Modes** | Normal simulation view, distraction-free Cinematic mode, and lower-third Capture mode for recording. |
| 🖥️ **Live Wallpaper Engine Integration** | Native Wallpaper Engine Workshop package with auto-pause, property bridges, and ambient HUD. |
| 🔗 **Seed & World Sharing** | Export/import reproducible worlds via compact `PP1.` URL sharing codes. |
| 🛡️ **Defensive Architecture** | Resilient storage layer with automatic QuotaExceeded fallbacks and crash recovery banners. |
| ⚡ **60 FPS High-DPI Rendering** | DPR-scaled Canvas 2D engine with adaptive performance scaling and long-frame watchdogs. |
| 📦 **Open Source & Local-First** | Zero tracking, 100% offline functionality, and optional Supabase cloud backups. |

---

## 🚀 Quick Install & Installation

### Option 1: Web & Desktop Application
```bash
# Clone the repository
git clone https://github.com/SahilKhutey/Primodilas.git
cd Primodilas

# Install dependencies and launch
npm install
npm run dev
```

- Open `http://localhost:5173` in your browser.
- Append `?wallpaper=1` for distraction-free ambient wallpaper mode.

### Option 2: Standalone Installers
Download pre-compiled native installers directly from [GitHub Releases](https://github.com/SahilKhutey/Primodilas/releases):
- **Windows**: `PolygonalPrimordials-Setup-1.0.0.exe`
- **Linux**: `polygonal-primordials_1.0.0_amd64.deb` / `AppImage`
- **macOS**: `PolygonalPrimordials-1.0.0.pkg`

---

## 🖥️ System Requirements

| Specification | Minimum | Recommended |
|---|---|---|
| **OS** | Windows 10/11, macOS 12+, Ubuntu 22.04+ | Windows 11 64-bit |
| **CPU** | 2.0 GHz Dual-Core 64-bit | 3.0+ GHz Quad-Core (AMD Ryzen / Intel Core) |
| **Memory** | 2 GB RAM | 4 GB RAM |
| **Graphics** | WebGL 2.0 / Canvas 2D compatible GPU | Dedicated GPU (NVIDIA RTX / AMD Radeon) |
| **Storage** | 200 MB available space | 500 MB available space |

---

## 🛠️ For Developers

### Prerequisites
- Node.js ≥ 20.0.0 & npm ≥ 10.0.0
- C++23 Compiler (GCC 13+, Clang 17+, MSVC 2022) & CMake ≥ 3.22 (for native research engine)

### Setup
```bash
# One-command development setup
npm install
./dev/setup_dev.sh
```

---

## 📦 Build & Packaging

```bash
# Build production web bundle
npm run build

# Build wallpaper mode bundle
npm run build:wallpaper

# Package for Wallpaper Engine Workshop
npm run package:wallpaper

# Package for itch.io distribution
npm run package:all
```

---

## 🧪 Automated Testing & QA

Polygonal Primordials maintains a 10-Tier Commercial QA Architecture:

```bash
# 1. Run unit, simulation, integration, fault injection, compatibility suites (38 files, 92 tests)
npm run test

# 2. Run full multi-tier test matrix
npm run test:matrix

# 3. Run Playwright Browser E2E suite (13 Chromium specs)
npm run test:e2e

# 4. Run automated performance benchmark
npm run benchmark

# 5. Run browser rendering benchmark
npm run benchmark:browser

# 6. Run physical hardware profiler (Windows)
powershell -ExecutionPolicy Bypass -File Scripts/hardware-report.ps1

# 7. Run full 12-stage Master Release Gate
npm run release:gate
```

---

## 📁 Project Architecture

```text
Primodilas/
├── src/                          # 🌐 Core Simulation & Web/Wallpaper UI
│   ├── components/               # React 18 HUD, overlays, dialogs, presentation chrome
│   ├── hooks/                    # Persistent settings, first-run, keyboard, presentation hooks
│   ├── lib/                      # Adaptive performance, safe storage, world sharing, diagnostics
│   ├── sim/                      # 30Hz deterministic simulation engine, entities, biomes, neural brains
│   └── styles/                   # Phase 5 presentation animations & Tailwind CSS
│
├── e2e/                          # 🎭 Playwright Browser E2E Test Suite (13 specs)
│   ├── fixtures/app.ts           # App boot and preparation fixtures
│   └── *.spec.ts                 # Startup, onboarding, controls, settings, sharing, recovery
│
├── tests/                        # 🧪 Vitest Test Suite (38 files, 92 tests)
│   ├── unit/                     # RNG, settings, storage, watchdog, autosave, themes
│   ├── simulation/               # Step invariants, initialization, determinism, biology
│   ├── integration/              # Deep snapshot/restore, runtime contracts, full smoke
│   ├── fault-injection/          # Quota errors, corrupt URLs, malformed snapshots
│   └── compatibility/            # Schema migration helpers, v1 snapshot decoding
│
├── Benchmarks/                   # 📊 Machine-Readable Performance Baselines & Reports
├── packaging/                    # 📦 Platform Packaging (Wallpaper Engine, itch.io)
├── release/                      # 📋 Release Candidate Notes, Checklists & Manifests
├── Tools/                        # 🛠️ Release Gate, Test Matrix & Benchmark Automation Scripts
└── docs/                         # 📖 QA, Release Pipeline & Architecture Specifications
```

---

## 📜 License

Distributed under the **MIT License**. See [LICENSE](LICENSE) for details.

```text
Copyright (c) 2024-2026 Sahil Khutey (Gensouls Lab)
```
