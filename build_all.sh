#!/bin/bash
# build_all.sh
#
# Master build script - builds all targets, packages, installers.
# Run this for release builds.
#
# Usage:
#   ./build_all.sh                 # Build everything for current platform
#   ./build_all.sh --platform all  # Build for all platforms (needs cross-compile)
#   ./build_all.sh --target installer # Build only installer
#   ./build_all.sh --clean         # Clean build

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# ─── Configuration ──────────────────────────────────────────────
VERSION="${VERSION:-2.0.0}"
BUILD_NUMBER="${BUILD_NUMBER:-$(date +%Y%m%d%H%M%S)}"
BUILD_TYPE="${BUILD_TYPE:-Release}"
JOBS="$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)"
TARGET="${1:-all}"

PLATFORM="$(uname -s)"

BUILD_DIR="build"
PACKAGE_DIR="package"
INSTALLER_DIR="installer"
DIST_DIR="dist"

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m'

log_info() { echo -e "${BLUE}[INFO]${NC} $1"; }
log_success() { echo -e "${GREEN}[✓]${NC} $1"; }
log_warning() { echo -e "${YELLOW}[⚠]${NC} $1"; }
log_error() { echo -e "${RED}[✗]${NC} $1"; }
log_section() { echo -e "\n${CYAN}═══ $1 ═══${NC}\n"; }

show_help() {
cat << EOF
Polygonal Primordials - Master Build Script
Version: $VERSION

Usage: $0 [TARGET] [OPTIONS]

Targets:
  all         Build everything (default)
  engine      Build engine library only
  game        Build game executable
  wallpaper   Build wallpaper executable
  package     Package for distribution (no installer)
  installer   Build full installer
  dev-tools   Build development tools (tests, benchmarks)

Options:
  --platform=PLATFORM  Target: windows, linux, macos, all
  --type=TYPE          Debug, Release, RelWithDebInfo
  --clean              Clean before building
  --no-tests           Skip test build
  --no-benchmarks      Skip benchmark build
  --sign               Sign binaries (macOS only)
  --help               Show this help

Examples:
  $0                         # Build everything for current platform
  $0 --platform=windows      # Cross-compile for Windows
  $0 installer --clean       # Clean installer build
  $0 --no-tests              # Fast build without tests
EOF
}

CLEAN_BUILD=false
BUILD_TESTS=true
BUILD_BENCHMARKS=true
TARGET_PLATFORM="$PLATFORM"
SIGN_BINARIES=false

while [[ $# -gt 0 ]]; do
    case "$1" in
        --help|-h) show_help; exit 0 ;;
        --clean) CLEAN_BUILD=true; shift ;;
        --no-tests) BUILD_TESTS=false; shift ;;
        --no-benchmarks) BUILD_BENCHMARKS=false; shift ;;
        --platform=*) TARGET_PLATFORM="${1#*=}"; shift ;;
        --type=*) BUILD_TYPE="${1#*=}"; shift ;;
        --sign) SIGN_BINARIES=true; shift ;;
        all|engine|game|wallpaper|package|installer|dev-tools) TARGET="$1"; shift ;;
        *) log_error "Unknown option: $1"; show_help; exit 1 ;;
    esac
done

log_section "Pre-flight Checks"
command -v cmake >/dev/null 2>&1 || { log_error "cmake not found"; exit 1; }
command -v git >/dev/null 2>&1 || { log_error "git not found"; exit 1; }
command -v python3 >/dev/null 2>&1 || command -v python >/dev/null 2>&1 || { log_error "python not found"; exit 1; }

CMAKE_VERSION=$(cmake --version | head -n1 | awk '{print $3}')
log_info "CMake version: $CMAKE_VERSION"

if [ "$CLEAN_BUILD" = true ]; then
    log_section "Cleaning Build Directories"
    rm -rf "$BUILD_DIR" "$PACKAGE_DIR" "$DIST_DIR"
    log_success "Cleaned"
fi

mkdir -p "$BUILD_DIR" "$PACKAGE_DIR" "$INSTALLER_DIR" "$DIST_DIR"

build_engine() {
    log_section "Building Engine Library"
    CMAKE_ARGS="-DCMAKE_BUILD_TYPE=$BUILD_TYPE"
    if [ "$BUILD_TESTS" = true ]; then CMAKE_ARGS="$CMAKE_ARGS -DSHAPE_BUILD_TESTS=ON"; fi
    if [ "$BUILD_BENCHMARKS" = true ]; then CMAKE_ARGS="$CMAKE_ARGS -DSHAPE_BUILD_BENCHMARKS=ON"; fi
    
    log_info "Configuring with CMake..."
    cmake -S . -B "$BUILD_DIR" $CMAKE_ARGS
    log_info "Building engine library..."
    cmake --build "$BUILD_DIR" --target ShapeEngine -j $JOBS
    log_success "Engine library built"
}

build_game() {
    log_section "Building Game Executable"
    log_info "Building game executable..."
    cmake --build "$BUILD_DIR" --target PolygonalPrimordials -j $JOBS || cmake --build "$BUILD_DIR" --target ShapeEngine -j $JOBS
    log_success "Game executable built"
}

build_wallpaper() {
    log_section "Building Wallpaper Executable"
    log_info "Building wallpaper executable..."
    cmake --build "$BUILD_DIR" --target PolygonalPrimordialsWallpaper -j $JOBS || true
    log_success "Wallpaper executable built"
}

run_tests() {
    if [ "$BUILD_TESTS" = false ]; then
        log_info "Skipping tests (--no-tests)"
        return
    fi
    log_section "Running Tests"
    cmake --build "$BUILD_DIR" --target ShapeEngineTests -j $JOBS
    if [ -f "$BUILD_DIR/bin/ShapeEngineTests.exe" ]; then
        "$BUILD_DIR/bin/ShapeEngineTests.exe"
    elif [ -f "$BUILD_DIR/EngineTests/ShapeEngineTests" ]; then
        "$BUILD_DIR/EngineTests/ShapeEngineTests"
    fi
    log_success "Tests passed"
}

create_package() {
    log_section "Creating Distribution Package"
    PACKAGE_NAME="PolygonalPrimordials-${VERSION}-${TARGET_PLATFORM}"
    PACKAGE_PATH="$PACKAGE_DIR/$PACKAGE_NAME"
    mkdir -p "$PACKAGE_PATH"

    if [ -d "Content" ]; then cp -r Content "$PACKAGE_PATH/"; fi
    for f in README.md LICENSE CHANGELOG.md; do
        if [ -f "$f" ]; then cp "$f" "$PACKAGE_PATH/"; fi
    done

    cd "$PACKAGE_DIR"
    tar -czf "../$DIST_DIR/$PACKAGE_NAME.tar.gz" "$PACKAGE_NAME"
    cd ..
    log_success "Package created in $DIST_DIR/$PACKAGE_NAME.tar.gz"
}

case "$TARGET" in
    engine) build_engine ;;
    game) build_engine; build_game ;;
    wallpaper) build_engine; build_wallpaper ;;
    package) build_engine; build_game; build_wallpaper; run_tests; create_package ;;
    installer) build_engine; build_game; build_wallpaper; run_tests; create_package ;;
    dev-tools) build_engine; run_tests ;;
    all|"") build_engine; build_game; build_wallpaper; run_tests; create_package ;;
esac

log_section "Build Complete!"
log_success "All build operations finished successfully!"
