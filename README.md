# Polygonal Primordials

> A living ecosystem simulation for your desktop

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)]() 
[![Version](https://img.shields.io/badge/version-2.0.0-blue)]() 
[![License](https://img.shields.io/badge/license-MIT-green)](LICENSE) 
[![Platforms](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)]()

---

## 🎮 For End Users

### Quick Install

| Platform | Download / Installer |
| :--- | :--- |
| **Windows** | [PolygonalPrimordials-Setup-2.0.0.exe](https://github.com/SahilKhutey/Primodials/releases) |
| **Linux (.deb)** | [polygonal-primordials_2.0.0_amd64.deb](https://github.com/SahilKhutey/Primodials/releases) |
| **Linux (Universal)** | [PolygonalPrimordials-2.0.0-x86_64.AppImage](https://github.com/SahilKhutey/Primodials/releases) |
| **macOS** | [PolygonalPrimordials-2.0.0.pkg](https://github.com/SahilKhutey/Primodials/releases) |

### System Requirements

- **OS**: Windows 10/11 (64-bit), Ubuntu 22.04+, macOS 12+
- **CPU**: Dual-core 2.0 GHz or faster
- **RAM**: 2 GB RAM (4 GB recommended for 250,000+ entities)
- **GPU**: OpenGL 3.3+ / Vulkan 1.2+ capable
- **Storage**: 200 MB available space

---

## 🛠️ For Developers

### Quick Start (One Command Setup)

```bash
git clone https://github.com/SahilKhutey/Primodials.git
cd Primodials
./dev/setup_dev.sh
```

This single command will:
1. Detect your platform and install system build dependencies
2. Create Python virtual environment for asset tools
3. Configure Git pre-commit & pre-push hooks
4. Set up VS Code configuration tasks & debug targets
5. Run the master build system & Catch2 test suite (190 test cases, 1,311 assertions)

### Build Commands

```bash
# Build everything for current platform
./build_all.sh all

# Build specific components
./build_all.sh engine     # ShapeEngine static library
./build_all.sh game       # Main simulation game binary
./build_all.sh wallpaper  # Live desktop wallpaper background mode
./build_all.sh package    # Build standalone tarball
./build_all.sh installer  # Build platform installer package (.exe / .deb / .pkg)
```

---

## 📄 Release Generation

To produce a full distributable release package with checksums:

```bash
./create_release.sh
```

---

## 📜 License & Community

- **Engine & Application**: MIT License — see [LICENSE](LICENSE).
- **GitHub Repository**: [`https://github.com/SahilKhutey/Primodials`](https://github.com/SahilKhutey/Primodials)
- **itch.io Store**: [`https://gensoulslab.itch.io/polygonal-primordials`](https://gensoulslab.itch.io/polygonal-primordials)
