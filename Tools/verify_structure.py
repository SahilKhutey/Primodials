#!/usr/bin/env python3
"""
verify_structure.py
Deep dive into project directory hierarchy and file counts.
"""

import sys
from pathlib import Path

def main():
    print("=" * 60)
    print("  PROJECT STRUCTURE VERIFICATION")
    print("=" * 60 + "\n")

    expected_dirs = [
        "Engine/Core", "Engine/Renderer", "Engine/ECS2", "Engine/Themes",
        "Engine/UI", "Engine/Localization", "Engine/Steam", "Engine/Diary",
        "Content/Themes", "Content/Locales", "docs", "tools", "EngineTests"
    ]

    for d in expected_dirs:
        p = Path(d)
        if p.is_dir():
            file_count = len(list(p.rglob("*")))
            print(f"  [OK] {d:25s} ({file_count:3d} files)")
        else:
            print(f"  [WARN] {d:25s} (MISSING)")

    print("\nSource File Distribution:")
    print(f"  .cpp:   {len(list(Path('.').rglob('*.cpp'))):4d}")
    print(f"  .hpp/.h:{len(list(Path('.').rglob('*.h*'))):4d}")
    print(f"  .json:  {len(list(Path('.').rglob('*.json'))):4d}")
    print(f"  .py:    {len(list(Path('.').rglob('*.py'))):4d}")
    print(f"  .md:    {len(list(Path('.').rglob('*.md'))):4d}")
    return 0

if __name__ == "__main__":
    sys.exit(main())
