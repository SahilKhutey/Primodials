#!/bin/bash
# build_pkg.sh - Build macOS .pkg installer
set -e

VERSION="${VERSION:-2.0.0}"
APP_NAME="PolygonalPrimordials"
IDENTIFIER="game.primordials.pp"
BUILD_DIR="${BUILD_DIR:-build}"
OUTPUT_DIR="${OUTPUT_DIR:-dist}"

echo "Building macOS .pkg for $APP_NAME v$VERSION"

command -v pkgbuild >/dev/null 2>&1 || { echo "WARNING: pkgbuild not found. Skipping macOS .pkg generation."; exit 0; }

STAGING="$BUILD_DIR/pkg-staging"
rm -rf "$STAGING"
mkdir -p "$STAGING/Applications"

if [ -d "$BUILD_DIR/PolygonalPrimordials.app" ]; then
    cp -R "$BUILD_DIR/PolygonalPrimordials.app" "$STAGING/Applications/"
fi

mkdir -p "$OUTPUT_DIR"
PRODUCT_PKG="$OUTPUT_DIR/PolygonalPrimordials-${VERSION}.pkg"

pkgbuild --root "$STAGING" \
         --identifier "$IDENTIFIER" \
         --version "$VERSION" \
         --install-location "/" \
         "$PRODUCT_PKG"

echo "✓ macOS .pkg created: $PRODUCT_PKG"
