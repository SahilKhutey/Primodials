#!/usr/bin/env python3
import sys
from pathlib import Path

def main():
    print("Verifying release structure...")
    required_files = [
        "CMakeLists.txt",
        "LICENSE",
        "README.md",
        "Marketing/store_page/description.txt",
        "Marketing/store_page/system_requirements.txt"
    ]
    all_ok = True
    for f in required_files:
        p = Path(f)
        if p.exists():
            print(f"  [OK] {f}")
        else:
            print(f"  [FAIL] {f}")
            all_ok = False
    
    if all_ok:
        print("Release verification PASSED!")
        return 0
    else:
        print("Release verification FAILED!")
        return 1

if __name__ == "__main__":
    sys.exit(main())
