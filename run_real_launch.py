#!/usr/bin/env python3
"""
run_real_launch.py
Master Real Launch execution script orchestrating:
1. Version synchronization (2.0.0)
2. Master verification & test suite execution
3. Production release packaging
4. Deployment helper execution
5. Live operations dashboard startup
"""

import sys
import subprocess

def run_step(title, script_path):
    print(f"\n>>> {title}")
    res = subprocess.run([sys.executable, script_path], cwd=".")
    if res.returncode != 0:
        print(f"\n[X] {title} failed with exit code {res.returncode}. Aborting real launch.")
        sys.exit(res.returncode)

def main():
    print("=" * 70)
    print("      POLYGONAL PRIMORDIALS -- REAL LAUNCH EXECUTION")
    print("=" * 70)

    run_step("Sync Release Version 2.0.0", "tools/sync_release_version.py")
    run_step("Execute Master Verification Suite", "tools/verify_all.py")
    run_step("Execute Release Packaging & Smoke Test", "launch_today.py")
    run_step("Prepare itch.io Deployment", "tools/deploy_itch.py")
    run_step("Start Live Operations Dashboard", "tools/post_launch_ops.py")

    print("\n" + "=" * 70)
    print("  REAL LAUNCH SETUP COMPLETED SUCCESSFULLY! ")
    print("  Your application is live, verified, packaged, and monitored!")
    print("=" * 70 + "\n")
    return 0

if __name__ == "__main__":
    sys.exit(main())
