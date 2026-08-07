# 19_Development_Workflow

This document outlines CMake build commands, test runner execution, and profiling setups.

---

## 1. Building the Code
The project uses CMake to configure and build executable targets:
* **Configuration:**
  ```powershell
  & "C:\Program Files\CMake\bin\cmake.exe" -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
  ```
* **Compilation:**
  ```powershell
  & "C:\Program Files\CMake\bin\cmake.exe" --build build
  ```

---

## 2. Running Verification Tests
All tests are executed via the compiled Catch2 test runner:
* **Running all tests:**
  ```powershell
  ./build/EngineTests/ShapeEngineTests.exe
  ```
* **Running specific tags:**
  ```powershell
  ./build/EngineTests/ShapeEngineTests.exe [Career]
  ```

---

## 3. Profiling & Performance Analysis
To run benchmarks and cache miss profile runs:
1. Compile the code under RelWithDebInfo configuration.
2. Run VTune or Valgrind tools targeting `ShapeEngineTests` to track allocation overheads and cache line hit rates.
