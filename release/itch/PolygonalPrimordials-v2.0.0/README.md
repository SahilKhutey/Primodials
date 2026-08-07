# Polygonal Primordials

> A data-oriented C++23 game engine + Lotka-Volterra artificial life simulation. Available as both a full game and a live wallpaper.

[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)
[![C++](https://img.shields.io/badge/C%2B%2B-23-blueviolet)](https://en.cppreference.com/w/cpp/23)
[![Platforms](https://img.shields.io/badge/platforms-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)]()

---

## 🎮 What Is It?

Polygonal Primordials is an **ecosystem simulation** where polygonal creatures with heritable traits evolve, hunt, and reproduce in real-time. Built from scratch in modern C++23 with a custom game engine, it demonstrates a production-grade archetype-based Entity-Component System capable of simulating 250,000+ entities at 60 FPS.

---

## 🌟 Features

### Two Products, One Engine
🎮 **Polygonal Primordials (Game)** — Full simulation with editor, save/load, multiplayer, achievements  
🖼️ **Polygonal Primordials Live Wallpaper** — Runs as your desktop background, atmospheric and ambient  

### Core Features
- **Real Evolution**: 8 heritable traits per creature
- **High Performance**: 250K entities @ 60 FPS verified
- **Open Source**: Engine under MIT license
- **Cross-Platform**: Windows, Linux, macOS
- **Multi-Language**: 11 languages supported (EN, ES, JA, FR, DE, ZH-CN, ZH-TW, PT-BR, RU, KO, IT)
- **Workshop**: Steam Workshop for community themes
- **OpenGL 4.6**: Modern graphics API with instanced rendering

---

## 🎯 Quick Start

### Play Now
🎮 **Game**: [Download on itch.io](https://gensoulslab.itch.io/polygonal-primordials)  

### Build from Source
```bash
git clone https://github.com/SahilKhutey/Primodials.git
cd Primodials
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release -j$(nproc)
./build/bin/PolygonalPrimordials
```
See [docs/QUICKSTART.md](docs/QUICKSTART.md) for detailed instructions.

---

## 🎮 Controls

| Key | Action |
|---|---|
| **WASD / Arrow Keys** | Pan camera |
| **Mouse Wheel** | Zoom in/out |
| **Space** | Pause/Resume |
| **Tab** | Toggle panels |
| **ESC** | Menu |
| **Left Click** | Select entity |
| **Right Click + Drag** | Box select |
| **F5** | Save game |
| **F9** | Load game |

---

## 🛠️ Tech Stack

- **Language**: C++23
- **Rendering**: OpenGL 4.6 (SDL3 Renderer)
- **Windowing**: SDL3
- **UI**: Custom Immediate Mode & ImGui
- **Audio**: SDL_mixer wrapper
- **Scripting**: Lua 5.4
- **Build System**: CMake / Ninja

---

## 📚 Documentation

- [docs/00_START_HERE.md](docs/00_START_HERE.md) — Navigation guide
- [docs/QUICKSTART.md](docs/QUICKSTART.md) — 5-minute setup guide
- [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) — System design deep-dive
- [docs/BUILD.md](docs/BUILD.md) — Build instructions for all platforms
- [docs/MODDING_GUIDE.md](docs/MODDING_GUIDE.md) — Create mods and themes
- [docs/THEMES.md](docs/THEMES.md) — Theme creation guide
- [docs/WORKSHOP.md](docs/WORKSHOP.md) — Steam Workshop guide
- [docs/API.md](docs/API.md) — Code API reference
- [docs/TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md) — Common issues & solutions
- [docs/FAQ.md](docs/FAQ.md) — Frequently asked questions

---

## 🎓 For Developers

The engine is open source under MIT. You can:
- Study the data-oriented architecture
- Use it in your own projects
- Contribute improvements
- Create mods and themes

See [CONTRIBUTING.md](CONTRIBUTING.md) for how to help.

---

## 📊 Performance

| Entity Count | FPS (RTX 3060) | RAM Usage |
|---|---|---|
| 10,000 | 60 | 50 MB |
| 50,000 | 60 | 150 MB |
| 100,000 | 60 | 250 MB |
| 250,000 | 60 | 500 MB |
| 500,000 | 30 | 950 MB |

---

## 🤝 Community & Links

- 💬 **Discord**: [Join Discord Server](https://discord.gg/primordials)
- 🐦 **Twitter**: [@SahilKhutey](https://twitter.com/SahilKhutey)
- 💻 **GitHub**: [github.com/SahilKhutey/Primodials](https://github.com/SahilKhutey/Primodials)

---

## 📜 License

This project is licensed under the MIT License — see [LICENSE](LICENSE) for details.

The primordial soup awaits. Welcome. 🧬
