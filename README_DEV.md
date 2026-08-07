# Polygonal Primordials — Developer Guide

Welcome to the developer documentation for **Polygonal Primordials**.

---

## 🛠️ Quick Developer Environment Setup

### 1. Automated Setup Script
Run the automated environment setup for your operating system:

```bash
# Linux / macOS
./dev/setup_dev.sh

# Windows (Command Prompt / PowerShell)
.\dev\setup_dev.bat
```

### 2. Manual Prerequisites
If installing manually:
- **C++23 Compiler**: GCC 13+, Clang 16+, or MSVC 2022+
- **Build System**: CMake 3.22+ & Ninja
- **Multimedia**: SDL3 (v3.2.4+)
- **Python**: Python 3.10+ with `Pillow`, `NumPy`, `PyYAML`

---

## 🏗️ Architecture & Engine Modules

- `Engine/Core/`: Job system, logging, arena/pool memory allocators, bootstrap.
- `Engine/ECS/`: Component storage, system execution scheduler, entity registry.
- `Engine/UI/`: Modern SDL3 immediate/retained UI framework (Menus, HUDs, Inspector, Theme Selector, Performance Graph, Achievements, Accessibility).
- `Engine/Localization/`: Dynamic locale manager, string table loader, parameter formatting.
- `Engine/Themes/`: Color palette generator, live theme switcher, gradient renderer.

---

## 🧪 Running the Test Suite

```bash
# Build and execute Catch2 unit test suite
build/bin/ShapeEngineTests.exe
```
