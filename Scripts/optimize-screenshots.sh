#!/usr/bin/env bash
# ─────────────────────────────────────────────
# Primordials — Screenshot Optimizer
# Compresses all PNGs in assets/screenshots/
# ─────────────────────────────────────────────
set -euo pipefail

SRC_DIR="${1:-assets/screenshots}"
OUT_DIR="${2:-assets/screenshots/web}"

if ! command -v cwebp &> /dev/null; then
  echo "❌ cwebp not installed. Install with: brew install webp / apt install webp"
  exit 1
fi

if ! command -v convert &> /dev/null; then
  echo "⚠️ ImageMagick not installed."
  exit 1
fi

mkdir -p "$OUT_DIR"
shopt -s nullglob

for png in "$SRC_DIR"/*.png; do
  filename=$(basename "$png" .png)
  
  # Generate 1920w WebP
  convert "$png" -resize 1920x -quality 90 "$OUT_DIR/${filename}-1920.webp"
  echo "✓ ${filename}-1920.webp"
  
  # Generate 1280w WebP (for smaller embeds)
  convert "$png" -resize 1280x -quality 85 "$OUT_DIR/${filename}-1280.webp"
  echo "✓ ${filename}-1280.webp"
  
  # Generate thumbnail 480w WebP
  convert "$png" -resize 480x -quality 80 "$OUT_DIR/${filename}-480.webp"
  echo "✓ ${filename}-480.webp"
  
  # Generate JPEG fallback
  convert "$png" -resize 1920x -quality 85 "$OUT_DIR/${filename}-1920.jpg"
  echo "✓ ${filename}-1920.jpg"
done

echo ""
echo "🎉 All screenshots optimized in $OUT_DIR"
