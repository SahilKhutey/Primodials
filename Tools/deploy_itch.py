#!/usr/bin/env python3
"""
deploy_itch.py
Automated deployment script for itch.io using Butler CLI or instructions.
"""

import sys
import subprocess
from pathlib import Path

USERNAME = "gensoulslab"
PROJECT = "polygonal-primordials"

def main():
    print("=" * 60)
    print("  AUTOMATED itch.io DEPLOYER")
    print("=" * 60 + "\n")

    rel_zip = Path("release/itch/PolygonalPrimordials-v2.0.0-win.zip")
    if not rel_zip.exists():
        print("  [X] Release archive not found! Run launch_today.py first.")
        return 1

    mb = rel_zip.stat().st_size / 1024 / 1024
    print(f"  Found package: {rel_zip.name} ({mb:.1f} MB)")

    # Check if butler CLI is installed
    butler = shutil_which("butler")
    if butler:
        target = f"{USERNAME}/{PROJECT}:windows-alpha"
        print(f"  Uploading via Butler to {target}...")
        res = subprocess.run([butler, "push", str(rel_zip), target])
        if res.returncode == 0:
            print("  [OK] Butler push succeeded!")
            return 0
    else:
        print("\n  Butler CLI not installed on local PATH.")
        print(f"  Manual upload instructions:")
        print(f"  1. Go to https://itch.io/dashboard")
        print(f"  2. Open project: {PROJECT}")
        print(f"  3. Upload file: {rel_zip.resolve()}")
        print(f"  4. Set platform: Windows (Executable)")
        print("\n  [OK] Ready for web deployment!")
        return 0

def shutil_which(cmd):
    import shutil
    return shutil.which(cmd)

if __name__ == "__main__":
    sys.exit(main())
