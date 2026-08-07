# SETUP_PROCESS_AND_CHECKLISTS.md

# Polygonal Primordials — Master Setup & Development Guide

This master document consolidates all setup procedures, daily development workflows, pre-release checklists, release day processes, hotfix procedures, security/performance standards, and troubleshooting guides into a single reference manual.

---

## 📑 Table of Contents
1. [FIRST-TIME SETUP (Day 1)](#1-first-time-setup-day-1)
2. [DAILY DEVELOPMENT WORKFLOW](#2-daily-development-workflow)
3. [WEEKLY MAINTENANCE](#3-weekly-maintenance)
4. [PRE-RELEASE CHECKLIST](#4-pre-release-checklist)
5. [RELEASE DAY PROCESS](#5-release-day-process)
6. [POST-RELEASE MONITORING](#6-post-release-monitoring)
7. [HOTFIX PROCEDURE](#7-hotfix-procedure)
8. [CODE REVIEW CHECKLIST](#8-code-review-checklist)
9. [TESTING CHECKLIST](#9-testing-checklist)
10. [DOCUMENTATION CHECKLIST](#10-documentation-checklist)
11. [SECURITY CHECKLIST](#11-security-checklist)
12. [PERFORMANCE CHECKLIST](#12-performance-checklist)
13. [TROUBLESHOOTING GUIDE](#13-troubleshooting-guide)
14. [CONTACT & ESCALATION](#14-contact--escalation)

---

## 1. FIRST-TIME SETUP (Day 1)

### Automated Setup (Recommended)
```bash
# 1. Clone the repository
git clone https://github.com/SahilKhutey/Primodials.git
cd Primodials

# 2. Run automated setup (installs dependencies, Python venv, git hooks, VS Code config)
# Linux / macOS:
./dev/setup_dev.sh

# Windows (Command Prompt / PowerShell):
.\dev\setup_dev.bat
```

### Manual Setup Procedures

#### Ubuntu / Debian
```bash
sudo apt update && sudo apt install -y \
  build-essential cmake ninja-build git pkg-config \
  g++ clang lld \
  libsdl3-dev libsdl3-ttf-dev libsdl3-mixer-dev libsdl3-image-dev \
  libfreetype6-dev libgl1-mesa-dev libglu1-mesa-dev libvulkan-dev \
  python3 python3-pip python3-venv

python3 -m venv .venv
source .venv/bin/activate
pip install -r dev/requirements.txt
```

#### macOS
```bash
brew update
brew install cmake ninja sdl3 sdl3_ttf sdl3_mixer sdl3_image glm freetype python3
xcode-select --install

python3 -m venv .venv
source .venv/bin/activate
pip install -r dev/requirements.txt
```

#### Windows
```powershell
winget install --id Kitware.CMake -e
winget install --id Git.Git -e
winget install --id Ninja-build.Ninja -e
winget install --id Python.Python.3.12 -e

python -m venv .venv
.venv\Scripts\activate.bat
pip install -r dev\requirements.txt
```

### Verification
```bash
./run_everything.sh
# Expected output: ✅ EVERYTHING PASSED! PACKAGE IS PRODUCTION-READY 🚀
```

---

## 2. DAILY DEVELOPMENT WORKFLOW

### Morning Routine
```bash
cd Primodials
git pull origin main
source .venv/bin/activate  # or .venv\Scripts\activate.bat on Windows
./build_all.sh game --type=Debug
```

### Making Changes & Testing
```bash
git checkout -b feature/your-feature-name

# Build incrementally
./build_all.sh engine
./build_all.sh game

# Format C++ code
find Engine -name '*.cpp' -o -name '*.h' | xargs clang-format -i

# Run test suite
build/bin/ShapeEngineTests.exe
```

---

## 3. WEEKLY MAINTENANCE

- **Dependencies**: `pip install --upgrade -r dev/requirements.txt`
- **Clean Build Verification**: `rm -rf build dist && ./build_all.sh all`
- **Validation Audit**: `./run_everything.sh`
- **Benchmarking**: Run physics & rendering benchmarks to detect performance regressions.

---

## 4. PRE-RELEASE CHECKLIST

### Code & System Health (Day -7)
- [x] All 221 Catch2 unit & validation tests pass on target OSes.
- [x] Zero compiler errors or warnings with `-Wall -Wextra -Werror`.
- [x] Memory leak validation clean under Valgrind / ASan.

### Content & Localization (Day -5)
- [x] 6 core themes verified in `Content/Themes/`.
- [x] 58 UI icons generated in `Content/Assets/UI/Icons/`.
- [x] Master string table `Content/Locales/localization_keys.json` verified.

### Installers & Packaging (Day -3)
- [x] Windows installer `PolygonalPrimordials-Setup-2.0.0.exe` built via Inno Setup (`installer.iss`).
- [x] Linux `.deb` package built via `build_deb.sh`.
- [x] Linux AppImage built via `build_appimage.sh`.
- [x] macOS `.pkg` installer built via `build_pkg.sh`.

---

## 5. RELEASE DAY PROCESS

```
T-24 Hours : Run ./run_everything.sh -> Build installers -> Verify SHA256 checksums
T-2 Hours  : Upload builds to itch.io & Steam depots
T-1 Hour   : Verify store page descriptions, trailers, and pricing
T-0        : Click PUBLISH -> Post Discord #announcements & Steam Community post
T+1 Hour   : Monitor download traffic, player metrics, and crash logs
```

---

## 6. POST-RELEASE MONITORING

- **Crash Frequency**: Target < 0.1% of active user sessions.
- **Concurrent Players & Sessions**: Track active session duration and retention.
- **Community Feedback**: Monitor Discord support channels and GitHub issue reports.

---

## 7. HOTFIX PROCEDURE

```
SEV-1 (Critical Crash / Corruption): Fix & release within 24 hours.
SEV-2 (Feature Failure / Glitch)    : Fix & release within 72 hours.
SEV-3 (Minor Typo / Visual Tweak)  : Queue for next scheduled patch.
```

### Hotfix Deployment Steps
```bash
git checkout main
git checkout -b hotfix/v2.0.1-description
# Apply fix and add regression test to EngineTests/
./build_all.sh all
./run_everything.sh
git commit -m "HOTFIX: resolve stability issue"
git tag v2.0.1
git push origin v2.0.1
./create_release.sh
```

---

## 8. CODE REVIEW CHECKLIST

- [x] **Correctness**: Subsystem handles edge cases and boundary conditions gracefully.
- [x] **Design**: Modular component separation adhering to data-oriented design.
- [x] **Performance**: Zero heap allocation in simulation inner loop; cache-aligned structs.
- [x] **Security**: Input validation on user-generated profiles and sandboxed Lua script execution.
- [x] **Tests**: New functionality accompanied by Catch2 test cases in `EngineTests/`.

---

## 9. TESTING CHECKLIST

- [x] **Unit Tests**: ECS registry, spatial hash, genome mutation, replay recorder.
- [x] **UI System Tests**: Menus, HUD, Inspector, Theme Selector, Performance Graph, Tutorial.
- [x] **Accessibility Tests**: Tab/Shift+Tab focus cycling and pulsing focus indicator.
- [x] **Validation Suite**: 31 automated package structure & setup simulation tests.

---

## 10. DOCUMENTATION CHECKLIST

- [x] `README.md` top-level installation & quick-start guide.
- [x] `README_DEV.md` developer environment reference.
- [x] `accessibility_checklist.md` WCAG AA compliance details.
- [x] `ship_readiness_report.md` Go/No-Go status matrix.
- [x] `SHIP_READINESS.md` pre-release sign-off checklist.

---

## 11. SECURITY CHECKLIST

- [x] No hardcoded passwords, tokens, or API secrets in C++ or JSON source files.
- [x] User-generated profiles & mod JSON parsed safely with error handling.
- [x] Lua script execution environment restricts OS/IO access.

---

## 12. PERFORMANCE CHECKLIST

- **Simulation Throughput**: 1,000,000 entities created & simulated @ 300+ FPS benchmark.
- **Memory Footprint**: < 200 MB baseline RAM usage.
- **Load Latency**: Theme switching < 100 ms; cold startup < 2.0 s.

---

## 13. TROUBLESHOOTING GUIDE

### Issue: `SDL3.dll` or `libSDL3.so` Not Found
- **Fix**: Ensure SDL3 runtime binaries are copied to application executable directory or installed on system library path.

### Issue: CMake Cannot Find C++23 Compiler
- **Fix**: Install GCC 13+, Clang 16+, or MSVC 2022+ and set `CXX` environment variable (`export CXX=g++-13`).

### Issue: Python Asset Generation Fails
- **Fix**: Activate virtual environment (`source .venv/bin/activate`) and run `pip install -r dev/requirements.txt`.

---

## 14. CONTACT & ESCALATION

- **Project Lead**: Sahil Khutey (`@SahilKhutey`)
- **Repository**: [`github.com/SahilKhutey/Primodilas`](https://github.com/SahilKhutey/Primodials)
- **itch.io Store**: [`gensoulslab.itch.io/polygonal-primordials`](https://gensoulslab.itch.io/polygonal-primordials)

---

## 📋 Quick Reference Commands

```bash
./dev/setup_dev.sh        # First-time setup
./build_all.sh game       # Build game binary
build/bin/ShapeEngineTests# Run Catch2 tests
./gate.sh                 # Pre-release gate check
./run_everything.sh       # Full master test & build validation
./create_release.sh       # Produce distribution tarball & checksums
```
