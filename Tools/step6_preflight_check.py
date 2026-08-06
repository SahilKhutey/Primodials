#!/usr/bin/env python3
"""
step6_preflight_check.py
Pre-flight go/no-go validation checklist script.
"""

import sys
import json
from pathlib import Path

def main():
    print("=" * 60)
    print("  STEP 6: PRE-FLIGHT CHECKLIST (Go / No-Go Decision)")
    print("=" * 60 + "\n")

    checks = [
        ("Game binary builds or exists", Path("build/bin/PolygonalPrimordials.exe").exists() or Path("build/PolygonalPrimordials.exe").exists()),
        ("Wallpaper binary builds or exists", Path("build/bin/PolygonalPrimordialsWallpaper.exe").exists() or Path("build/PolygonalPrimordialsWallpaper.exe").exists()),
        ("Deep Sea theme present", Path("Content/Themes/deep-sea/theme.json").exists()),
        ("Coral Reef theme present", Path("Content/Themes/coral-reef/theme.json").exists()),
        ("Aurora theme present", Path("Content/Themes/aurora/theme.json").exists()),
        ("README.md exists", Path("README.md").exists()),
        ("LICENSE exists", Path("LICENSE").exists()),
        ("All 11 locale JSONs valid", all(Path(f"Content/Locales/{loc}.json").exists() for loc in ["en", "es", "ja"])),
        ("itch.io package archive exists", len(list(Path("release/itch").glob("*.zip"))) > 0 if Path("release/itch").exists() else False)
    ]

    passed = 0
    failed = 0

    for name, status in checks:
        icon = "[OK]" if status else "[X]"
        print(f"  {icon} {name}")
        if status:
            passed += 1
        else:
            failed += 1

    print("\n" + "=" * 60)
    print(f"  SUMMARY: {passed} passed, {failed} failed")
    print("=" * 60)

    if failed == 0:
        print("\n  ============================================")
        print("        READY TO LAUNCH")
        print("  ============================================")
        return 0
    else:
        print("\n  ============================================")
        print("        NOT READY - FIX CRITICAL ISSUES")
        print("  ============================================")
        return 1

if __name__ == "__main__":
    sys.exit(main())
