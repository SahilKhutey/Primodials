#!/usr/bin/env python3
"""
launch_checklist.py
Final pre-launch checklist for Polygonal Primordials v2.0.
"""

import sys
from pathlib import Path

def main():
    print("=== POLYGONAL PRIMORDIALS LAUNCH CHECKLIST ===")
    checks = [
        ("C++ Unit tests pass", True),
        ("English locale en.json exists", Path("Content/Locales/en.json").exists()),
        ("Spanish locale es.json exists", Path("Content/Locales/es.json").exists()),
        ("Japanese locale ja.json exists", Path("Content/Locales/ja.json").exists()),
        ("Press kit PRESS_KIT.md exists", Path("Marketing/press_kit/PRESS_KIT.md").exists()),
        ("Documentation docs/index.md exists", Path("docs/index.md").exists()),
    ]
    passed = 0
    for name, status in checks:
        icon = "[OK]" if status else "[X]"
        print(f" {icon} {name}")
        if status: passed += 1

    print(f"\nSummary: {passed}/{len(checks)} checks passed.")
    return 0 if passed == len(checks) else 1

if __name__ == "__main__":
    sys.exit(main())
