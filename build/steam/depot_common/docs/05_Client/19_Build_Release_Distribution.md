# 19_Build_Release_Distribution

This document specifies the deployment build settings, target platform packaging configurations, and assets copying scripts.

---

## 1. Release Package Layout
Deployable releases compile assets and binaries into a clean package:

```
Primodilas_Release/
 ├── Primodilas.exe         — Client game executable
 ├── ShapeEngineTests.exe   — Verification test runner
 ├── assets/                — Outfit fonts, sound loops
 └── docs/                  — Embedded Simulation Bible docs
```

---

## 2. CMake Packaging Commands
The build scripts use CPACK commands to bundle targets:
```cmake
set(CPACK_PACKAGE_NAME "Polygonal_Primordials")
set(CPACK_PACKAGE_VERSION "1.0.0")
include(CPack)
```

---

## 3. Deployment Compilations
To ensure optimal client performance, build pipelines compile targets with `-O3` optimizations, stripping debug symbols and bundling standard library DLL dependencies into the release directory coordinates.
