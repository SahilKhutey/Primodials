#!/usr/bin/env python3
"""
step3_build_all.py
Build all targets for game and wallpaper engines in Release mode.
"""

import sys
import subprocess
from pathlib import Path

def main():
    print("=" * 60)
    print("  STEP 3: BUILD FOR ALL TARGETS")
    print("=" * 60 + "\n")

    cmd = ["cmake", "--build", "build", "--config", "Release"]
    print(f"Executing: {' '.join(cmd)}")
    
    result = subprocess.run(cmd, cwd=".")
    if result.returncode == 0:
        print("\n  [OK] Build completed successfully")
        return 0
    else:
        print(f"\n  [X] Build failed with exit code {result.returncode}")
        return 1

if __name__ == "__main__":
    sys.exit(main())
