#!/bin/bash
# Install build dependencies for macOS
set -e

echo "Installing dependencies for macOS..."

if ! command -v brew >/dev/null 2>&1; then
    echo "Homebrew not found. Install from https://brew.sh"
    exit 1
fi

brew update
brew install cmake ninja pkg-config git sdl3 sdl3_ttf sdl3_mixer sdl3_image glm freetype python3

echo "✓ All macOS dependencies installed"
