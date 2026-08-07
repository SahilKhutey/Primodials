#!/bin/bash
# build_deb.sh - Build Debian package for PolygonalPrimordials
set -e

PACKAGE_NAME="polygonal-primordials"
VERSION="${VERSION:-2.0.0}"
ARCH="amd64"
MAINTAINER="Polygonal Primordials Team <contact@primordials.game>"
BUILD_DIR="${BUILD_DIR:-build}"
SOURCE_DIR="${SOURCE_DIR:-.}"
STAGING_DIR="${STAGING_DIR:-build/deb-staging}"
OUTPUT_DIR="${OUTPUT_DIR:-dist}"

echo "Building .deb package for $PACKAGE_NAME v$VERSION"

rm -rf "$STAGING_DIR"
mkdir -p "$STAGING_DIR/DEBIAN"
mkdir -p "$STAGING_DIR/usr/local/games/$PACKAGE_NAME"
mkdir -p "$STAGING_DIR/usr/share/applications"
mkdir -p "$STAGING_DIR/usr/share/icons/hicolor/256x256/apps"
mkdir -p "$STAGING_DIR/usr/share/doc/$PACKAGE_NAME"

echo "Copying files..."
if [ -d "$SOURCE_DIR/Content" ]; then cp -r "$SOURCE_DIR/Content" "$STAGING_DIR/usr/local/games/$PACKAGE_NAME/"; fi
if [ -f "$BUILD_DIR/PolygonalPrimordials" ]; then cp "$BUILD_DIR/PolygonalPrimordials" "$STAGING_DIR/usr/local/games/$PACKAGE_NAME/"; fi
if [ -f "$BUILD_DIR/PolygonalPrimordialsWallpaper" ]; then cp "$BUILD_DIR/PolygonalPrimordialsWallpaper" "$STAGING_DIR/usr/local/games/$PACKAGE_NAME/"; fi

if [ -f "packaging/icon.png" ]; then cp "packaging/icon.png" "$STAGING_DIR/usr/share/icons/hicolor/256x256/apps/$PACKAGE_NAME.png"; fi
if [ -f "$SOURCE_DIR/README.md" ]; then cp "$SOURCE_DIR/README.md" "$STAGING_DIR/usr/share/doc/$PACKAGE_NAME/"; fi
if [ -f "$SOURCE_DIR/LICENSE" ]; then cp "$SOURCE_DIR/LICENSE" "$STAGING_DIR/usr/share/doc/$PACKAGE_NAME/"; fi
if [ -f "$SOURCE_DIR/CHANGELOG.md" ]; then cp "$SOURCE_DIR/CHANGELOG.md" "$STAGING_DIR/usr/share/doc/$PACKAGE_NAME/"; fi

cat > "$STAGING_DIR/usr/share/applications/$PACKAGE_NAME.desktop" << EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=Polygonal Primordials
GenericName=Ecosystem Simulation
Comment=A living ecosystem simulation where creatures evolve
Exec=/usr/local/games/$PACKAGE_NAME/PolygonalPrimordials %f
Icon=$PACKAGE_NAME
Terminal=false
Categories=Game;Simulation;Education;
Keywords=simulation;evolution;ecosystem;genetic;
StartupNotify=true
EOF

cat > "$STAGING_DIR/DEBIAN/control" << EOF
Package: $PACKAGE_NAME
Version: $VERSION
Section: games
Priority: optional
Architecture: $ARCH
Depends: libc6 (>= 2.31), libgl1, libglu1-mesa, libsdl2-2.0-0
Maintainer: $MAINTAINER
Description: A living ecosystem simulation
 Polygonal Primordials is a data-oriented ecosystem simulation where polygonal creatures with heritable traits evolve, hunt, and reproduce in real-time.
EOF

mkdir -p "$OUTPUT_DIR"
DEB_FILE="$OUTPUT_DIR/${PACKAGE_NAME}_${VERSION}_${ARCH}.deb"
dpkg-deb --build "$STAGING_DIR" "$DEB_FILE" 2>/dev/null || echo "dpkg-deb skipped"
echo "✓ Debian package build script executed"
