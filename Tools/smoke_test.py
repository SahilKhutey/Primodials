#!/usr/bin/env python3
"""
smoke_test.py
Run the compiled game binary for 3 seconds to verify it opens and initializes without crashing.
"""

import sys
import time
import subprocess
from pathlib import Path

def main():
    print("=" * 60)
    print("  STEP 2: QUICK SMOKE TEST")
    print("=" * 60 + "\n")

    game_exe = Path("build/itch-release/bin/PolygonalPrimordials.exe")
    if not game_exe.exists():
        game_exe = Path("build/itch-release/PolygonalPrimordials.exe")
    if not game_exe.exists():
        game_exe = Path("build/bin/PolygonalPrimordials.exe")

    if not game_exe.exists():
        print("  [X] Game binary PolygonalPrimordials.exe not found!")
        return 1

    print(f"  Launching {game_exe} for 3 seconds...")
    try:
        proc = subprocess.Popen([str(game_exe)], cwd=".")
        time.sleep(3)
        poll = proc.poll()
        if poll is None:
            print("  [OK] Game launched and is running smoothly (no immediate crash)")
            proc.terminate()
            proc.wait(timeout=2)
            return 0
        else:
            print(f"  [X] Game exited prematurely with code {poll}")
            return 1
    except Exception as e:
        print(f"  [X] Failed to launch game: {e}")
        return 1

if __name__ == "__main__":
    sys.exit(main())
