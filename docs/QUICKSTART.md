# Quick Start Guide

Get Polygonal Primordials running in **5 minutes**.

---

## 🎮 Option 1: Download Pre-Built (Easiest)

### Download
Get the latest release for your platform:  
🎮 **Game**: [itch.io](https://gensoulslab.itch.io/polygonal-primordials)  
🖼️ **Wallpaper**: [itch.io](https://gensoulslab.itch.io/polygonal-primordials-wallpaper)  

Supported platforms:
- **Windows**: Windows 10+ (64-bit)
- **Linux**: Ubuntu 20.04+, or similar
- **macOS**: macOS 11+ (Big Sur or later)

---

## 🛠️ Option 2: Build from Source (5-15 minutes)

### Prerequisites
- C++23 Compiler (GCC 13+, Clang 16+, or MSVC 19.34+)
- CMake 3.22+
- Git

### Build Steps
```bash
# 1. Clone repository
git clone https://github.com/SahilKhutey/Primodials.git
cd Primodials

# 2. Configure build
cmake -G Ninja -S . -B build -DCMAKE_BUILD_TYPE=Release

# 3. Build executable
cmake --build build --config Release

# 4. Run
./build/bin/PolygonalPrimordials.exe
```

---

## 🎮 Basic Controls

| Action | Key/Mouse |
|---|---|
| Pan camera | WASD or Arrow Keys |
| Zoom in/out | Mouse Wheel |
| Pause/Resume | Space |
| Speed control | 1-9 (1=slow, 9=fast) |
| Toggle panels | Tab |
| Main menu | ESC |
| Select entity | Left Click |
| Box select | Right Click + Drag |
| Save | F5 |
| Load | F9 |

---

## 📚 Next Steps

- 🎨 [THEMES.md](THEMES.md) — Create custom themes
- 🧬 [MODDING_GUIDE.md](MODDING_GUIDE.md) — Create Lua mods
- 🏗️ [ARCHITECTURE.md](ARCHITECTURE.md) — Understand the engine
- 🤝 [CONTRIBUTING.md](CONTRIBUTING.md) — Contribute to the project
