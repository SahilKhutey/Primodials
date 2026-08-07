#!/bin/bash
# create_release.sh
#
# Creates a complete distributable release package.
# Run this to generate the final release files.

set -e

VERSION="${VERSION:-2.0.0}"
TIMESTAMP=$(date +%Y%m%d)
RELEASE_NAME="PolygonalPrimordials-v${VERSION}-${TIMESTAMP}"
RELEASE_DIR="dist-release-${VERSION}"

echo "Creating release: $RELEASE_NAME"

./build_all.sh all

rm -rf "$RELEASE_DIR"
mkdir -p "$RELEASE_DIR/source"
mkdir -p "$RELEASE_DIR/binaries/$(uname -s)"
mkdir -p "$RELEASE_DIR/installers"
mkdir -p "$RELEASE_DIR/documentation"

echo "Copying source..."
if [ -d ".git" ]; then
    tar -czf "$RELEASE_DIR/source.tar.gz" \
        --exclude='build' \
        --exclude='dist' \
        --exclude='.venv' \
        --exclude='*.o' \
        --exclude='*.so' \
        --exclude='*.dll' \
        --exclude='*.exe' \
        .
else
    cp -r Engine Content Games Tools "$RELEASE_DIR/source/" 2>/dev/null || true
fi

echo "Copying binaries..."
PLATFORM_DIR="$RELEASE_DIR/binaries/$(uname -s)"
cp build/Release/PolygonalPrimordials.exe "$PLATFORM_DIR/" 2>/dev/null || cp build/PolygonalPrimordials "$PLATFORM_DIR/" 2>/dev/null || true
cp build/Release/PolygonalPrimordialsWallpaper.exe "$PLATFORM_DIR/" 2>/dev/null || cp build/PolygonalPrimordialsWallpaper "$PLATFORM_DIR/" 2>/dev/null || true
if [ -d "Content" ]; then cp -r Content "$PLATFORM_DIR/"; fi

echo "Copying installers..."
if [ -d "dist" ]; then cp -r dist/* "$RELEASE_DIR/installers/" 2>/dev/null || true; fi

echo "Copying documentation..."
cp README.md "$RELEASE_DIR/documentation/" 2>/dev/null || true
cp LICENSE "$RELEASE_DIR/documentation/" 2>/dev/null || true
cp CHANGELOG.md "$RELEASE_DIR/documentation/" 2>/dev/null || true
cp accessibility_checklist.md "$RELEASE_DIR/documentation/" 2>/dev/null || true
cp ship_readiness_report.md "$RELEASE_DIR/documentation/" 2>/dev/null || true

echo "Generating checksums..."
cd "$RELEASE_DIR"
find . -type f \( -name "*.exe" -o -name "*.so*" -o -name "*.dll" -o -name "*.deb" -o -name "*.pkg" -o -name "*.AppImage" -o -name "*.zip" -o -name "*.tar.gz" \) \
    -exec sha256sum {} \; > SHA256SUMS 2>/dev/null || true
cd ..

cat > "$RELEASE_DIR/README.txt" << EOF
Polygonal Primordials v${VERSION} - Release Package
====================================================

CONTENTS:
  binaries/     - Pre-built binaries ($(uname -s))
  installers/   - Platform installers
  documentation/ - User and developer docs
  SHA256SUMS    - File checksums

DOCUMENTATION:
  See documentation/ directory
  Or visit: https://github.com/SahilKhutey/Primodials

BUILD DATE: $(date)
VERSION: ${VERSION}
EOF

tar -czf "$RELEASE_NAME.tar.gz" "$RELEASE_DIR"
sha256sum "$RELEASE_NAME.tar.gz" > "$RELEASE_NAME.tar.gz.sha256" 2>/dev/null || true

RELEASE_SIZE=$(du -sh "$RELEASE_NAME.tar.gz" | cut -f1)

echo ""
echo "╔═══════════════════════════════════════════════════════════════╗"
echo "║                                                               ║"
echo "║  ✓ Release package created!                                   ║"
echo "║                                                               ║"
echo "║  File: $RELEASE_NAME.tar.gz                                  "
echo "║  Size: $RELEASE_SIZE                                          "
echo "║                                                               ║"
echo "╚═══════════════════════════════════════════════════════════════╝"
echo ""
