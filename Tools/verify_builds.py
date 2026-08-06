#!/usr/bin/env python3
"""
verify_builds.py
Audit compiled build artifacts and executables.
"""

import sys
from pathlib import Path

def main():
    print("=" * 60)
    print("  BUILD ARTIFACTS AUDIT")
    print("=" * 60 + "\n")

    build_dir = Path("build")
    if not build_dir.exists():
        print("  [X] build/ directory does not exist")
        return 1

    bins = [
        "bin/PolygonalPrimordials.exe", "PolygonalPrimordials.exe", "PolygonalPrimordials",
        "bin/PolygonalPrimordialsWallpaper.exe", "PolygonalPrimordialsWallpaper.exe", "PolygonalPrimordialsWallpaper",
        "bin/ShapeEngineTests.exe", "ShapeEngineTests.exe", "EngineTests"
    ]

    for b in bins:
        p = build_dir / b
        if p.exists():
            mb = p.stat().st_size / 1024 / 1024
            print(f"  [OK] Found executable: {b:35s} ({mb:.1f} MB)")

    objs = list(build_dir.rglob("*.obj")) + list(build_dir.rglob("*.o"))
    print(f"\n  Compiled object files: {len(objs)}")
    return 0

if __name__ == "__main__":
    sys.exit(main())
