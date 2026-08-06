#!/usr/bin/env python3
"""
step4_run_tests.py
Runs the full Catch2 test suite.
"""

import sys
import subprocess
from pathlib import Path

def main():
    print("=" * 60)
    print("  STEP 4: RUN FULL TEST SUITE")
    print("=" * 60 + "\n")

    test_exe = Path("build/bin/ShapeEngineTests.exe")
    if not test_exe.exists():
        test_exe = Path("build/ShapeEngineTests.exe")

    if not test_exe.exists():
        print("  [X] Test binary ShapeEngineTests not found!")
        return 1

    cmd = [str(test_exe), "[wallpaper],[Wallpaper],[settings],[diary],[shared],[workshop],[autostart],[validator],[publisher],[ratings],[locale],[FinalQA],[FullSuite]"]
    print(f"Executing: {' '.join(cmd)}")

    result = subprocess.run(cmd, cwd=".")
    if result.returncode == 0:
        print("\n  [OK] All test cases passed")
        return 0
    else:
        print(f"\n  [X] Test suite failed with exit code {result.returncode}")
        return 1

if __name__ == "__main__":
    sys.exit(main())
