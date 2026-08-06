#!/usr/bin/env python3
"""
crash_log_analyzer.py
Parses and summarizes crash logs from Content/_user/logs/crash.log.
"""

import sys
from pathlib import Path

def main():
    print("=" * 60)
    print("  CRASH LOG ANALYZER")
    print("=" * 60 + "\n")

    crash_file = Path("Content/_user/logs/crash.log")
    if not crash_file.exists():
        print("  [OK] No crash log file found (Content/_user/logs/crash.log does not exist).")
        return 0

    content = crash_file.read_text(encoding="utf-8", errors="ignore").strip()
    if not content:
        print("  [OK] Crash log file is empty. Zero crashes reported.")
        return 0

    print("  [WARN] Crash Log Content:")
    print("-" * 60)
    print(content)
    print("-" * 60)
    return 0

if __name__ == "__main__":
    sys.exit(main())
