#!/bin/bash
# Launch_Primordials.sh
# One-click launcher for Primordials on Linux & macOS

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

echo "==============================================================="
echo "               🚀 Launching Primordials...                    "
echo "==============================================================="

if [ ! -f "build/bin/PolygonalPrimordials" ] && [ ! -f "build/itch-release/bin/PolygonalPrimordials.exe" ]; then
    echo "[INFO] Game binary not found. Building Primordials..."
    ./build_all.sh game
fi

if [ -f "build/bin/PolygonalPrimordials" ]; then
    ./build/bin/PolygonalPrimordials &
elif [ -f "build/itch-release/bin/PolygonalPrimordials.exe" ]; then
    ./build/itch-release/bin/PolygonalPrimordials.exe &
fi
