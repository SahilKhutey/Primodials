#!/bin/bash
# run_everything.sh - The single command that tests everything

set -e
cd "$(dirname "${BASH_SOURCE[0]}")"

echo "╔═══════════════════════════════════════════════════════════════╗"
echo "║                                                               ║"
echo "║   Polygonal Primordials - Complete Validation                 ║"
echo "║                                                               ║"
echo "╚═══════════════════════════════════════════════════════════════╝"

echo "Running pre-release gate..."
./gate.sh

echo "Running master build and test suite..."
./build_all.sh all

echo ""
echo "✅ EVERYTHING PASSED! PACKAGE IS PRODUCTION-READY 🚀"
