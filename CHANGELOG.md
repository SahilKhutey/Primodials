# Changelog

All notable changes to Primordials will be documented in this file.
The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Complete brand visual identity (logo mark, Space Grotesk typography, color tokens)
- 6 biome specifications and JSON schema
- Comprehensive documentation suite (`ARCHITECTURE.md`, `BIOME_SCHEMA.md`, `CONTRIBUTING_BIOMES.md`, `FAQ.md`, `ROADMAP.md`)
- GitHub community files (issue templates, PR template, discussions guide)
- Multi-platform packaging pipeline (`npm run package:all`)

### Changed
- Root `README.md` completely overhauled with modern layout, clean badges, and clear installation paths
- Repository description, topics, and about panel updated

### Fixed
- License clarity (verified MIT)

---

## [2.0.0] - 2026-07-04

### Added
- Web application (`src/`) as primary consumer product
- React 18 + TypeScript + Vite + Tailwind CSS stack
- Canvas 2D high-performance rendering pipeline
- Optional Supabase cloud-save integration (100% offline-first by default)
- Wallpaper mode (`?wallpaper=1`)
- Steam Wallpaper Engine packaging scripts
- Unified build system: `build_all.sh` / `build_all.bat`
- Cross-platform native installers: Windows `.exe`, Linux `.deb`, Linux `AppImage`, macOS `.pkg`
- 221 Catch2 unit test cases, 1,311 assertions in C++ engine
- SDL3 integration and lockstep deterministic networking protocol
- Lotka-Volterra differential equation simulation core

---

## [1.0.0] - 2024-08-14

### Added
- Initial C++ ECS simulation engine
- Custom shape rendering (polygonal organisms)
- Catch2 unit test suite
- CMake build system
- SDL3 rendering backend
- Basic predator-prey dynamics
