#!/bin/bash
# build_unix_depots.sh
# Build Linux and macOS depots

set -e
VERSION="${VERSION:-2.0.0}"
APP_ID="${APP_ID:-1234560}"

echo "============================================================"
echo " Building Unix Depots for Polygonal Primordials v${VERSION}"
echo "============================================================"

mkdir -p build/steam/depot_linux
mkdir -p build/steam/depot_macos

if [ -f "build/bin/PolygonalPrimordialsWallpaper" ]; then
    cp "build/bin/PolygonalPrimordialsWallpaper" "build/steam/depot_linux/"
    echo " ✓ Linux binary copied"
fi

echo "Unix depots build complete."
