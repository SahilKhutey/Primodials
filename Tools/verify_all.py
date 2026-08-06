#!/usr/bin/env python3
"""
verify_all.py
Master runner executing all verification sub-tools sequentially.
"""

import sys
import subprocess

def run_script(script_name):
    print(f"\nRunning {script_name}...")
    res = subprocess.run([sys.executable, f"tools/{script_name}"], cwd=".")
    if res.returncode != 0:
        print(f"Warning: {script_name} returned code {res.returncode}")

def main():
    print("=" * 65)
    print("      POLYGONAL PRIMORDIALS -- COMPLETE MASTER SUITE")
    print("=" * 65)

    run_script("master_verify.py")
    run_script("verify_structure.py")
    run_script("verify_content.py")
    run_script("verify_docs.py")
    run_script("verify_git.py")
    run_script("verify_builds.py")
    run_script("health_summary.py")

    print("=" * 65)
    print("  ALL VERIFICATION CHECKS COMPLETED")
    print("=" * 65 + "\n")
    return 0

if __name__ == "__main__":
    sys.exit(main())
