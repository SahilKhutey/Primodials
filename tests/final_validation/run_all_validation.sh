#!/bin/bash
# run_all_validation.sh
# Runs all final validation tests.

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/../.."

BUILD_DIR="build/test-results"
REPORT_FILE="$BUILD_DIR/validation_report.txt"

mkdir -p "$BUILD_DIR"

echo "Running Final Validation Suite..."

if [ -f "build/bin/ShapeEngineTests.exe" ]; then
    "build/bin/ShapeEngineTests.exe" "[validation]"
elif [ -f "build/itch-release/bin/ShapeEngineTests.exe" ]; then
    "build/itch-release/bin/ShapeEngineTests.exe" "[validation]"
fi

echo "✓ Validation complete!"
