#!/usr/bin/env python3
"""
health_summary.py
One-page project health dashboard and decision matrix.
"""

import sys
from pathlib import Path

def main():
    print("=" * 65)
    print("         POLYGONAL PRIMORDIALS -- HEALTH SUMMARY")
    print("=" * 65)

    cpp_files = list(Path(".").rglob("*.cpp"))
    h_files = list(Path(".").rglob("*.h*"))
    loc = 0
    for p in cpp_files + h_files:
        if ".git" not in p.parts and "build" not in p.parts:
            loc += len(p.read_text(encoding="utf-8", errors="ignore").splitlines())

    themes = len(list(Path("Content/Themes").iterdir())) if Path("Content/Themes").exists() else 0
    locales = len(list(Path("Content/Locales").glob("*.json"))) if Path("Content/Locales").exists() else 0
    screenshots = len(list(Path("Marketing").rglob("*.png"))) + len(list(Path("Marketing").rglob("*.jpg"))) if Path("Marketing").exists() else 0
    docs = len(list(Path(".").rglob("*.md")))

    print(f"  Source:      {len(cpp_files) + len(h_files)} files, {loc:,} lines of code")
    print(f"  Themes:      {themes} themes")
    print(f"  Locales:     {locales} languages")
    print(f"  Screenshots: {screenshots} images")
    print(f"  Docs:        {docs} markdown files")

    print("-" * 65)
    crit = 0
    if not Path("README.md").exists(): crit += 1
    if not Path("LICENSE").exists(): crit += 1
    if not Path("Content/Themes/deep-sea").exists(): crit += 1

    if crit == 0:
        print("  Status: [OK] READY FOR LAUNCH")
        print("  Decision: Launch immediately! All critical checks passed.")
    else:
        print(f"  Status: [X] {crit} CRITICAL ISSUES DETECTED")

    print("=" * 65 + "\n")
    return 0

if __name__ == "__main__":
    sys.exit(main())
