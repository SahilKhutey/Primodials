# Troubleshooting Guide

Common issues and solutions.

---

## 🎮 Launch Issues

### Windows: "DLL not found"
- Install Visual C++ Redistributable 2022.

### Linux: "libSDL3.so not found"
```bash
sudo apt install libsdl3-dev
```

### macOS: "App is damaged and can't be opened"
```bash
xattr -cr /path/to/PolygonalPrimordials.app
```

---

## ⚡ Performance Fixes

- Lower target FPS (e.g., 30 FPS).
- Lower population density in Settings → Simulation.
- Enable auto-pause in background settings.
