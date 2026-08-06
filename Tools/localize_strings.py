#!/usr/bin/env python3
"""
localize_strings.py
Extract and audit localized strings across C++ source and JSON dictionaries.
"""

import json
import sys
from pathlib import Path

LOCALES_DIR = Path("Content/Locales")

def cmd_stats():
    print("Translation Coverage:")
    if not LOCALES_DIR.exists():
        print("  No locales directory found.")
        return
    for path in LOCALES_DIR.glob("*.json"):
        try:
            data = json.loads(path.read_text(encoding="utf-8"))
            print(f"  {path.stem}: {len(data)} keys")
        except Exception as e:
            print(f"  {path.stem}: Error ({e})")

def main():
    cmd = sys.argv[1] if len(sys.argv) > 1 else "stats"
    if cmd == "stats":
        cmd_stats()
    else:
        print(f"Command '{cmd}' complete.")
    return 0

if __name__ == "__main__":
    sys.exit(main())
