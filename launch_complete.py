#!/usr/bin/env python3
"""
launch_complete.py
Runs the complete 7-step launch validation and build pipeline in sequence.
"""

import sys
import subprocess
from pathlib import Path

def run_step(step_num, title, script_path):
    print(f"\n>>> STEP {step_num}/7: {title}")
    result = subprocess.run([sys.executable, script_path], cwd=".")
    if result.returncode != 0:
        print(f"\n[X] Step {step_num} failed with code {result.returncode}. Aborting launch sequence.")
        sys.exit(result.returncode)

def main():
    print("=" * 70)
    print("  POLYGONAL PRIMORDIALS -- COMPLETE LAUNCH SEQUENCE")
    print("=" * 70)

    run_step(1, "Code Health Check", "tools/step1_code_health.py")
    run_step(2, "Build All Release Targets", "tools/step3_build_all.py")
    run_step(3, "Run Full Test Suite", "tools/step4_run_tests.py")
    run_step(4, "Package for itch.io", "tools/step5_package_itch.py")
    run_step(5, "Pre-Flight Checklist", "tools/step6_preflight_check.py")
    run_step(6, "Generate Launch Readiness Report", "tools/generate_launch_report.py")
    run_step(7, "Launch Execution & Promotion", "tools/step7_launch.py")

    print("\n" + "=" * 70)
    print("  ALL LAUNCH PIPELINE STEPS PASSED SUCCESSFULLY! ")
    print("=" * 70 + "\n")
    return 0

if __name__ == "__main__":
    sys.exit(main())
