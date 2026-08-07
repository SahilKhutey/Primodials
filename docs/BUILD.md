# Build Instructions

Detailed build instructions for Windows, Linux, and macOS.

---

## 📋 Prerequisites

- **Compiler**:
  - Windows: MSVC 19.34+ (Visual Studio 2022) or Clang/LLVM
  - Linux: GCC 13+ or Clang 16+
  - macOS: Apple Clang 15+ (Xcode 15+)
- **Build System**: CMake 3.22+ & Ninja
- **OpenGL**: OpenGL 4.6 compatible drivers

---

## 🛠️ Quick Build

```bash
git clone https://github.com/SahilKhutey/Primodials.git
cd Primodials
cmake -G Ninja -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

---

## 💻 Windows Build

```cmd
cmake -G Ninja -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
build\bin\PolygonalPrimordials.exe
```

---

## 🐧 Linux Build

```bash
sudo apt update && sudo apt install -y build-essential cmake ninja-build libgl1-mesa-dev libglu1-mesa-dev libasound2-dev
cmake -G Ninja -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
./build/bin/PolygonalPrimordials
```

---

## 🍎 macOS Build

```bash
brew install cmake ninja sdl3
cmake -G Ninja -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
open ./build/bin/PolygonalPrimordials.app
```
