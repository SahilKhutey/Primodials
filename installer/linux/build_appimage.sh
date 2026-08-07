#!/bin/bash
# build_appimage.sh - Build universal Linux AppImage
set -e

VERSION="${VERSION:-2.0.0}"
APP_NAME="PolygonalPrimordials"
BUILD_DIR="${BUILD_DIR:-build}"
SOURCE_DIR="${SOURCE_DIR:-.}"
OUTPUT_DIR="${OUTPUT_DIR:-dist}"
APPDIR="$BUILD_DIR/AppDir"

echo "Building AppImage for $APP_NAME v$VERSION"

command -v appimagetool >/dev/null 2>&1 || { echo "WARNING: appimagetool not found, skipping AppImage compilation"; exit 0; }

rm -rf "$APPDIR"
mkdir -p "$APPDIR/usr/bin"
mkdir -p "$APPDIR/usr/lib/$APP_NAME"
mkdir -p "$APPDIR/usr/share/applications"
mkdir -p "$APPDIR/usr/share/icons/hicolor/256x256/apps"

if [ -f "$BUILD_DIR/PolygonalPrimordials" ]; then cp "$BUILD_DIR/PolygonalPrimordials" "$APPDIR/usr/lib/$APP_NAME/"; fi
if [ -d "$SOURCE_DIR/Content" ]; then cp -r "$SOURCE_DIR/Content" "$APPDIR/usr/lib/$APP_NAME/"; fi
if [ -f "packaging/icon.png" ]; then cp "packaging/icon.png" "$APPDIR/$APP_NAME.png"; fi

cat > "$APPDIR/AppRun" << 'EOF'
#!/bin/bash
APPDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
exec "$APPDIR/usr/lib/PolygonalPrimordials/PolygonalPrimordials" "$@"
EOF
chmod +x "$APPDIR/AppRun"

cat > "$APPDIR/usr/share/applications/$APP_NAME.desktop" << EOF
[Desktop Entry]
Type=Application
Name=Polygonal Primordials
Exec=AppRun
Icon=$APP_NAME
Categories=Game;Simulation;
Terminal=false
EOF

mkdir -p "$OUTPUT_DIR"
APPIMAGE_FILE="$OUTPUT_DIR/PolygonalPrimordials-${VERSION}-x86_64.AppImage"
cd "$BUILD_DIR"
ARCH=x86_64 appimagetool "$APPDIR" "$APPIMAGE_FILE"
cd ..
echo "✓ AppImage created: $APPIMAGE_FILE"
