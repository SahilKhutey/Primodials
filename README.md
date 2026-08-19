<!-- markdownlint-configure-file { "MD013": false, "MD033": false, "MD041": false }-->
<div align="center">

<img src="primordials-publish/assets/logo/logo-primary.svg" alt="Primordials" width="160"/>

# Primordials
### A living ecosystem on your desktop.

[![MIT License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)
[![Tests](https://img.shields.io/badge/Tests-221_passing-brightgreen.svg)](#-testing)
[![itch.io](https://img.shields.io/badge/itch.io-Play%20Online-fa5c5c.svg)](https://gensoulslab.itch.io/polygonal-primordials)
[![Discord](https://img.shields.io/badge/Discord-Join%20Community-5865F2.svg)](https://discord.gg/primordials)
[![GitHub release](https://img.shields.io/github/v/release/SahilKhutey/Primodials)](https://github.com/SahilKhutey/Primodials/releases)

**Real Lotka-Volterra predator-prey dynamics running at 60 FPS on your desktop. Free, open-source, MIT licensed.**

[**Download**](https://github.com/SahilKhutey/Primodials/releases) • [**Wallpaper Engine**](https://steamcommunity.com/sharedfiles/filedetails/?id=XXXXX) • [**itch.io**](https://gensoulslab.itch.io/polygonal-primordials) • [**Discord**](https://discord.gg/primordials)

</div>

---

## 🧬 What is Primordials?

**Primordials** is a real-time artificial life simulation that turns your monitor into a window into a primordial world.

Inspired by the classic **Lotka-Volterra predator-prey equations** taught in ecology, every organism you see follows real rules:
- 🐛 **Prey** graze on nutrients, reproduce, and flee from threats
- 🦈 **Predators** hunt, starve, and reproduce when nourished
- ⚖️ **The system finds balance** — or collapses dramatically

It runs as a **live desktop wallpaper**, **screensaver**, or **standalone windowed laboratory**, and ships with six biomes ranging from a peaceful pond to a volcanic deep-sea vent.

---

## ✨ Features

| Feature | Description |
|:---|:---|
| 🧬 **Real Ecology** | Lotka-Volterra dynamics with continuous differential equation solving. |
| 🌿 **6 Dynamic Biomes** | Algae Bloom, Deep Sea, Primordial Soup, Tundra, Coral Reef, and Volcanic Vent. |
| ⚡ **250K+ Entities** | Up to 250,000+ simultaneous organisms at 60 FPS in native C++ engine. |
| 🎨 **Bioluminescent Vector Art** | Distinctive polygonal aesthetic with customizable glow intensity. |
| 🖥️ **3 Display Modes** | Live Wallpaper, Ambient Screensaver, and Interactive Sandbox Window. |
| 💾 **100% Offline by Default** | Private and local-first; optional Supabase cloud saves. |
| 🛠️ **Custom Biomes** | Extensible JSON schema for designing custom species and parameters. |
| 📦 **Open Source** | Fully MIT licensed, zero telemetry, zero trackers. |

---

## 🚀 Quick Start

### 1. Web & Desktop Wallpaper (React + TypeScript)

```bash
# Clone the repository
git clone https://github.com/SahilKhutey/Primodials.git
cd Primodials

# Install dependencies and start development server
npm install
npm run dev
```

* Open `http://localhost:5173` for the interactive sandbox.
* Add `?wallpaper=1` to the URL or click **Wallpaper Mode** in the HUD for ambient playback.

### 2. Standalone Installers

| Platform | Download / Installer |
| :--- | :--- |
| **Windows** | [PolygonalPrimordials-Setup-2.0.0.exe](https://github.com/SahilKhutey/Primodials/releases) |
| **Linux (.deb)** | [polygonal-primordials_2.0.0_amd64.deb](https://github.com/SahilKhutey/Primodials/releases) |
| **Linux (Universal)** | [PolygonalPrimordials-2.0.0-x86_64.AppImage](https://github.com/SahilKhutey/Primodials/releases) |
| **macOS** | [PolygonalPrimordials-2.0.0.pkg](https://github.com/SahilKhutey/Primodials/releases) |

---

## 🛠️ For Developers (Native C++ Engine)

### Prerequisites
* C++23 compiler (GCC 13+, Clang 17+, MSVC 19.30+)
* CMake ≥ 3.25
* SDL3 development libraries
* Node.js ≥ 18 (for web app)
* Python ≥ 3.10 (for build scripts)

### One-Command Setup

```bash
./dev/setup_dev.sh
```

### Build Commands

```bash
./build_all.sh engine     # ShapeEngine static library
./build_all.sh game       # Main simulation game binary
./build_all.sh wallpaper  # Live desktop wallpaper mode
./build_all.sh package    # Standalone release tarball
./build_all.sh installer  # Platform installer package
```

### Run Test Suite

```bash
./build_all.sh engine && ctest --test-dir build/test --output-on-failure
```
*221 test cases · 1,311 assertions · 0 failures*

---

## 🧪 Architecture

Primordials is built on two complementary pillars:

```
Primodials/
├── src/                          # 🌐 Wallpaper & Web Application (Primary Product)
│   ├── components/               # React 18 / Tailwind HUD & controls
│   ├── sim/                      # TypeScript simulation runtime & Canvas 2D renderer
│   └── styles/                   # Design tokens & color system
│
├── Engine/ & Games/              # ⚡ Native C++23 Engine (Research Platform)
│   ├── Engine/                   # Custom ECS, spatial partitioning, lockstep netcode
│   └── EngineTests/              # Catch2 unit + integration tests (221 cases)
│
└── packaging/                    # 📦 Platform Packaging (Wallpaper Engine, InnoSetup)
```

See [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) for full architectural specifications.

---

## 🌿 The Biomes

| Biome | Population Dynamics | Visual Atmosphere |
| :--- | :--- | :--- |
| 🌱 **Algae Bloom** | High prey density, sparse grazers | Peaceful pond surface at dawn |
| 🐙 **Deep Sea** | Low density, bioluminescent hunters | Abyssal darkness 2km below |
| 🍲 **Primordial Soup** | Emerging, unstable protocells | Ancient ocean 4 billion years ago |
| ❄️ **Tundra** | Rapid boom-and-bust population cycles | Stark Arctic summer tundra |
| 🪸 **Coral Reef** | Dense multi-species equilibrium | Vibrant tropical coral sanctuary |
| 🌋 **Volcanic Vent** | Thermal-resistant extremophiles | Geothermal vents & sulfur beds |

* 🛠️ Biome schema: [docs/BIOME_SCHEMA.md](docs/BIOME_SCHEMA.md)
* 🌿 Contribute a biome: [docs/CONTRIBUTING_BIOMES.md](docs/CONTRIBUTING_BIOMES.md)

---

## 🤝 Contributing

Contributions are welcome! Please check:
* 🐛 [Bug Report](.github/ISSUE_TEMPLATE/bug_report.md)
* 💡 [Feature Request](.github/ISSUE_TEMPLATE/feature_request.md)
* 🌿 [Biome Idea](.github/ISSUE_TEMPLATE/biome_idea.md)
* 📖 [Contributing Guide](CONTRIBUTING.md)
* 📜 [Code of Conduct](CODE_OF_CONDUCT.md)

---

## 📜 License

Distributed under the **MIT License**. See [LICENSE](LICENSE) for details.

```
Copyright (c) 2024-2026 Sahil Khutey (Gensouls Lab)
```

---

<div align="center">
  <sub>Made with 🧬 by <a href="https://gensoulslab.dev/">Gensouls Lab</a></sub>
</div>
