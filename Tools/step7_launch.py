#!/usr/bin/env python3
"""
step7_launch.py
Final launch execution script with promotion templates.
"""

import sys
from pathlib import Path

PROMO_REDDIT = """Title: [Dev] Polygonal Primordials - Ecosystem simulation with real genetic algorithms (C++23, open source)

Hi r/playmygame! I'm a solo developer who built Polygonal Primordials - a data-oriented ecosystem simulation in C++23.

Features:
- Custom ECS handling 250K entities @ 60 FPS
- Real genetic algorithms (8 heritable traits)
- Watch emergent behavior (evolution, speciation, extinction)
- Cross-platform (Windows/Linux/macOS)
- Open source engine (MIT license)

Play free: https://gensouls.itch.io/polygonal-primordials
Source: https://github.com/SahilKhutey/Primodials

Feedback welcome!
"""

PROMO_TWITTER = """Just launched Polygonal Primordials on itch.io!

A real ecosystem simulation in C++23 where you watch creatures evolve through actual genetic algorithms.

250K entities @ 60 FPS
Heritable traits & real evolution
Cross-platform
Free (or pay what you want)

https://gensouls.itch.io/polygonal-primordials
#indiedev #gamedev #simulation #cpp
"""

def main():
    print("=" * 60)
    print("  STEP 7: LAUNCH EXECUTION & PROMOTION TEMPLATES")
    print("=" * 60 + "\n")

    release_dir = Path("release/itch")
    zips = list(release_dir.glob("*.zip")) if release_dir.exists() else []

    if zips:
        print("  [OK] Found release packages for distribution:")
        for z in zips:
            size_mb = z.stat().st_size / 1024 / 1024
            print(f"    - {z.name} ({size_mb:.1f} MB)")
    else:
        print("  [WARN] No release packages found in release/itch/")

    print("\n" + "=" * 60)
    print("  REDDIT PROMOTION TEMPLATE")
    print("=" * 60)
    print(PROMO_REDDIT)

    print("=" * 60)
    print("  TWITTER PROMOTION TEMPLATE")
    print("=" * 60)
    print(PROMO_TWITTER)

    print("=" * 60)
    print("    YOU ARE READY TO LAUNCH! SHIP IT! ")
    print("=" * 60 + "\n")
    return 0

if __name__ == "__main__":
    sys.exit(main())
