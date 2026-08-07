#!/usr/bin/env python3
"""
download_fonts.py

Downloads official Roboto fonts from Google Fonts repository into assets/fonts/ and Content/Assets/Fonts/.
"""

import os
import sys
import urllib.request
from pathlib import Path

FONTS = {
    "Roboto-Regular.ttf": "https://raw.githubusercontent.com/google/fonts/main/ofl/roboto/Roboto-Regular.ttf",
    "Roboto-Bold.ttf": "https://raw.githubusercontent.com/google/fonts/main/ofl/roboto/Roboto-Bold.ttf",
    "RobotoMono-Regular.ttf": "https://raw.githubusercontent.com/google/fonts/main/ofl/robotomono/static/RobotoMono-Regular.ttf",
}

DIRS = [
    Path("assets/fonts"),
    Path("Content/Assets/Fonts"),
    Path("build/itch-release/bin/Content/Assets/Fonts"),
    Path("build/Release/Content/Assets/Fonts"),
]

def main():
    print("=== Downloading Roboto Fonts ===")
    for d in DIRS:
        d.mkdir(parents=True, exist_ok=True)
    
    for filename, url in FONTS.items():
        print(f"Fetching {filename}...")
        try:
            req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
            with urllib.request.urlopen(req) as resp:
                font_bytes = resp.read()
                
            for d in DIRS:
                target_path = d / filename
                with open(target_path, "wb") as f:
                    f.write(font_bytes)
                print(f"  [+] Saved {filename} to {target_path} ({len(font_bytes)} bytes)")
        except Exception as e:
            print(f"  [-] Failed to download {filename}: {e}")
            # If download fails, fallback to local system font copying if available
            win_font = Path("C:/Windows/Fonts/arial.ttf")
            if win_font.exists():
                with open(win_font, "rb") as f_src:
                    data = f_src.read()
                for d in DIRS:
                    target_path = d / filename
                    with open(target_path, "wb") as f_dst:
                        f_dst.write(data)
                print(f"  [+] Fallback copied system Arial font to {filename} ({len(data)} bytes)")

if __name__ == "__main__":
    main()
