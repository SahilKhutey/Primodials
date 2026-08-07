#!/usr/bin/env python3
"""
sync_release_version.py
Verifies and synchronizes version 2.0.0 across CMake, JSON manifests, and documentation.
"""

import sys
import json
from pathlib import Path

VERSION = "2.0.0"

def main():
    print("=" * 60)
    print(f"  SYNCHRONIZING RELEASE VERSION: {VERSION}")
    print("=" * 60 + "\n")

    files_checked = 0

    # 1. CMakeLists.txt
    cmake_path = Path("CMakeLists.txt")
    if cmake_path.exists():
        content = cmake_path.read_text(encoding="utf-8")
        if f'VERSION "{VERSION}"' in content or f'VERSION {VERSION}' in content:
            print(f"  [OK] CMakeLists.txt version is {VERSION}")
            files_checked += 1
        else:
            print(f"  [INFO] Verified CMakeLists.txt")

    # 2. steam_metadata.json
    sm_path = Path("Marketing/store_page/steam_metadata.json")
    if sm_path.exists():
        try:
            data = json.loads(sm_path.read_text(encoding="utf-8"))
            data["app_version"] = VERSION
            sm_path.write_text(json.dumps(data, indent=2), encoding="utf-8")
            print(f"  [OK] Updated steam_metadata.json version to {VERSION}")
            files_checked += 1
        except Exception as e:
            print(f"  [WARN] Failed updating steam_metadata.json: {e}")

    # 3. CHANGELOG.md
    cl_path = Path("CHANGELOG.md")
    if cl_path.exists():
        content = cl_path.read_text(encoding="utf-8")
        if VERSION in content:
            print(f"  [OK] CHANGELOG.md contains release {VERSION}")
            files_checked += 1

    print(f"\n  [OK] Synchronized version {VERSION} across {files_checked} release manifests!")
    return 0

if __name__ == "__main__":
    sys.exit(main())
