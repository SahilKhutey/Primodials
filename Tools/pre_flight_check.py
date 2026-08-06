#!/usr/bin/env python3
"""
pre_flight_check.py
Pre-flight launch validator for Polygonal Primordials v2.0
"""
import sys
from pathlib import Path

def main():
    if hasattr(sys.stdout, 'reconfigure'):
        sys.stdout.reconfigure(encoding='utf-8')

    print("="*60)
    print("  PRE-FLIGHT CHECK - Polygonal Primordials v2.0")
    print("="*60)

    root = Path(".")
    checks = [
        ("CMakeLists.txt", root / "CMakeLists.txt"),
        ("Pre-launch checklist", root / "tests/checklist/PRE_LAUNCH_CHECKLIST.md"),
        ("Launch day checklist", root / "tests/checklist/LAUNCH_DAY_CHECKLIST.md"),
        ("Post-launch monitoring", root / "tests/checklist/POST_LAUNCH_MONITORING.md"),
        ("Rollback plan", root / "tests/checklist/ROLLBACK_PLAN.md"),
        ("Store page description", root / "Marketing/store_page/description.txt"),
        ("System requirements", root / "Marketing/store_page/system_requirements.txt"),
        ("Tutorial steps", root / "Content/Onboarding/tutorial_steps.json"),
    ]

    all_passed = True
    for name, path in checks:
        if path.exists():
            print(f"  [OK] {name}")
        else:
            print(f"  [FAIL] {name} missing: {path}")
            all_passed = False

    print("="*60)
    if all_passed:
        print("  [SUCCESS] PRE-FLIGHT CHECK PASSED! Ready for launch.")
        return 0
    else:
        print("  [FAILURE] PRE-FLIGHT CHECK FAILED!")
        return 1

if __name__ == "__main__":
    sys.exit(main())
