#!/usr/bin/env python3
"""
upload_to_steam.py

Upload built depots to Steamworks using SteamPipe.
"""

import os
import sys
import argparse
from pathlib import Path

def main():
    parser = argparse.ArgumentParser(description="Upload to Steamworks")
    parser.add_argument("--app-id", type=int, default=1234560, help="Steam App ID")
    parser.add_argument("--depot-version", default="2.0.0", help="Version")
    parser.add_argument("--dry-run", action="store_true", help="Dry run mode")
    args = parser.parse_args()

    print(f"\n{'='*70}")
    print(f"  STEAMWORKS UPLOAD PIPELINE — App ID: {args.app_id}")
    print(f"{'='*70}")

    depot_path = Path("build/steam")
    if not depot_path.exists():
        print("  Error: build/steam does not exist. Run tools/build_depots.py first.")
        return 1

    if args.dry_run:
        print("  [DRY RUN] SteamPipe upload simulated successfully.")
        print(f"  Depot VDF: {depot_path / 'app_build.vdf'}")
    else:
        print("  SteamPipe upload ready. (Run with steamcmd --run_app_build build/steam/app_build.vdf)")

    return 0

if __name__ == "__main__":
    sys.exit(main())
