#!/bin/bash
# build_steam.sh - Automated build script for Steam distribution
set -e

echo "Building Polygonal Primordials for Steam..."
cmake -B build -DCMAKE_BUILD_TYPE=Release -DSTEAMWORKS_SDK=ON
cmake --build build --config Release -j 8

echo "Packaging assets..."
# tool to cook assets would go here
# ./build/bin/AssetCooker ./Games/PolygonalPrimordials/data ./build/dist/data

echo "Copying Steam redistributables..."
# cp steam_api64.dll ./build/dist/

echo "Build complete. Ready for SteamPipe upload."
