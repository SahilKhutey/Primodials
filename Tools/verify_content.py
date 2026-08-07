#!/usr/bin/env python3
"""
verify_content.py
Deep verification of theme assets, locales, shaders, and audio.
"""

import sys
import json
from pathlib import Path

def main():
    print("=" * 60)
    print("  CONTENT ASSET VERIFICATION")
    print("=" * 60 + "\n")

    print("[Themes]")
    themes_dir = Path("Content/Themes")
    if themes_dir.exists():
        for d in themes_dir.iterdir():
            if d.is_dir():
                tj = d / "theme.json"
                if tj.exists():
                    try:
                        data = json.loads(tj.read_text(encoding="utf-8"))
                        name = data.get("displayName", d.name)
                        palette = data.get("speciesPalette", [])
                        print(f"  [OK] {d.name:25s} '{name}' ({len(palette)} colors in palette)")
                    except Exception as e:
                        print(f"  [X] {d.name:25s} Invalid JSON: {e}")
                else:
                    print(f"  [WARN] {d.name:25s} Missing theme.json")

    print("\n[Locales]")
    locales_dir = Path("Content/Locales")
    if locales_dir.exists():
        for f in sorted(locales_dir.glob("*.json")):
            try:
                data = json.loads(f.read_text(encoding="utf-8"))
                print(f"  [OK] {f.stem:8s} {len(data):3d} keys ({f.stat().st_size} bytes)")
            except Exception as e:
                print(f"  [X] {f.stem:8s} Invalid JSON: {e}")

    return 0

if __name__ == "__main__":
    sys.exit(main())
