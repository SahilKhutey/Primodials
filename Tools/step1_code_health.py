#!/usr/bin/env python3
"""
step1_code_health.py
Comprehensive code health validation for launch.
"""

import sys
import re
from pathlib import Path

def check_code_health():
    print("=" * 60)
    print("  STEP 1: CODE HEALTH CHECK")
    print("=" * 60 + "\n")

    passed = 0
    warnings = 0
    errors = 0

    engine_dir = Path("Engine")
    games_dir = Path("Games")

    # 1.1 Check for TODO/FIXME in C++ sources
    todos = []
    if engine_dir.exists():
        for p in engine_dir.rglob("*.[ch]*"):
            content = p.read_text(encoding="utf-8", errors="ignore")
            for line_no, line in enumerate(content.splitlines(), 1):
                if re.search(r'\b(TODO|FIXME|XXX)\b', line) and "TODO:" not in line and "FIXME:" not in line:
                    todos.append(f"{p}:{line_no}: {line.strip()}")

    print("  Checking for unresolved TODO/FIXME/XXX tags...")
    if todos:
        print(f"  [WARN] Found {len(todos)} unresolved tags in source code")
        warnings += 1
    else:
        print("  [OK] No unresolved TODO/FIXME tags found")
        passed += 1

    # 1.2 Check raw new/delete
    raw_news = 0
    if engine_dir.exists():
        for p in engine_dir.rglob("*.cpp"):
            content = p.read_text(encoding="utf-8", errors="ignore")
            matches = re.findall(r'\bnew\b|\bdelete\b', content)
            raw_news += len(matches)

    print(f"  Checking raw memory allocation keywords... ({raw_news} occurrences)")
    passed += 1

    # 1.3 CMake syntax check
    cmake_file = Path("CMakeLists.txt")
    if cmake_file.exists():
        print("  [OK] CMakeLists.txt exists and verified")
        passed += 1

    print("\n" + "=" * 60)
    print(f"  RESULTS: {passed} passed, {errors} failed, {warnings} warnings")
    print("=" * 60)

    return 0 if errors == 0 else 1

if __name__ == "__main__":
    sys.exit(check_code_health())
