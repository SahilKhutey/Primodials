#!/usr/bin/env python3
"""
organize_images.py
Organizes all game icons, banners, screenshots, and press assets into separate dedicated folders.
"""

import os
import sys
import shutil
from pathlib import Path

def main():
    print("=" * 60)
    print("  ORGANIZING GAME IMAGES, ICONS & BANNERS INTO SEPARATE FOLDERS")
    print("=" * 60 + "\n")

    base_brain = Path(r"C:\Users\ASUS\.gemini\antigravity\brain\03f7f6ca-f1b7-47a2-87a9-68b6f527545a")

    # Define target directories
    dirs = {
        "icons": Path("Marketing/icons"),
        "banners": Path("Marketing/banners"),
        "screenshots": Path("Marketing/screenshots"),
        "release_icons": Path("release/assets/icons"),
        "release_banners": Path("release/assets/banners"),
        "release_screenshots": Path("release/assets/screenshots")
    }

    for d in dirs.values():
        d.mkdir(parents=True, exist_ok=True)

    mappings = [
        # Icons
        (base_brain / "app_icon_1786001086121.jpg", dirs["icons"] / "app_icon_1024x1024.jpg"),
        (base_brain / "app_icon_1786001086121.jpg", dirs["icons"] / "game_icon_512x512.jpg"),
        (base_brain / "app_icon_1786001086121.jpg", dirs["icons"] / "discord_icon_512x512.jpg"),
        (base_brain / "app_icon_1786001086121.jpg", dirs["release_icons"] / "cover_icon.jpg"),

        # Banners
        (base_brain / "store_cover_178600102323.jpg" if (base_brain / "store_cover_178600102323.jpg").exists() else base_brain / "store_cover_1786001102323.jpg", dirs["banners"] / "store_cover_16x9.jpg"),
        (base_brain / "wallpaper_banner_1786001116055.jpg", dirs["banners"] / "wallpaper_banner_16x9.jpg"),
        (base_brain / "itch_banner_1786001137340.jpg", dirs["banners"] / "itch_banner_3x2.jpg"),
        (base_brain / "header_capsule_1786001275216.jpg", dirs["banners"] / "header_capsule_460x215.jpg"),
        (base_brain / "store_cover_1786001102323.jpg", dirs["release_banners"] / "store_cover.jpg"),

        # Screenshots
        (base_brain / "screenshot_heatmap_1786001208868.jpg", dirs["screenshots"] / "screenshot_01_heatmap.jpg"),
        (base_brain / "screenshot_speciation_1786001224881.jpg", dirs["screenshots"] / "screenshot_02_speciation.jpg"),
        (base_brain / "screenshot_inspector_1786001241808.jpg", dirs["screenshots"] / "screenshot_03_inspector.jpg"),
        (base_brain / "screenshot_aurora_1786001256690.jpg", dirs["screenshots"] / "screenshot_04_aurora.jpg"),
        (base_brain / "screenshot_heatmap_1786001208868.jpg", dirs["release_screenshots"] / "screenshot_01.jpg"),
        (base_brain / "screenshot_speciation_1786001224881.jpg", dirs["release_screenshots"] / "screenshot_02.jpg"),
        (base_brain / "screenshot_inspector_1786001241808.jpg", dirs["release_screenshots"] / "screenshot_03.jpg"),
        (base_brain / "screenshot_aurora_1786001256690.jpg", dirs["release_screenshots"] / "screenshot_04.jpg"),
    ]

    copied = 0
    for src, dst in mappings:
        if src.exists():
            shutil.copy2(src, dst)
            print(f"  [OK] Copied to {dst}")
            copied += 1
        else:
            print(f"  [WARN] Source file not found: {src.name}")

    print("\n" + "=" * 60)
    print(f"  Successfully organized {copied} image files into separate folders!")
    print("=" * 60 + "\n")
    return 0

if __name__ == "__main__":
    sys.exit(main())
