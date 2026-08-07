#!/usr/bin/env python3
"""
generate_loading_assets.py

Generates production-grade splash screen images, logos, and loading bar textures
for LoadingScreen in assets/ui/ and Content/Assets/UI/.
"""

import os
from pathlib import Path

DIRS = [
    Path("assets/ui"),
    Path("Content/Assets/UI"),
    Path("build/itch-release/bin/Content/Assets/UI"),
]

def generate_assets():
    from PIL import Image, ImageDraw, ImageFont, ImageFilter
    
    for d in DIRS:
        d.mkdir(parents=True, exist_ok=True)
    
    print("=== Generating Loading & Splash Screen Assets ===")
    
    # 1. Splash / Title Screen Image (1280x720)
    splash_size = (1280, 720)
    splash_img = Image.new("RGBA", splash_size, (10, 15, 25, 255))
    draw = ImageDraw.Draw(splash_img)
    
    # Draw ambient glowing radial background
    for r in range(400, 0, -10):
        alpha = int(25 * (1.0 - r / 400.0))
        draw.ellipse([640 - r, 360 - r, 640 + r, 360 + r], fill=(40, 100, 180, alpha))
        
    # Draw biological cell nodes connected by glowing lines
    nodes = [(300, 200), (450, 400), (700, 250), (850, 480), (1000, 300), (550, 550)]
    for i in range(len(nodes)):
        for j in range(i + 1, len(nodes)):
            draw.line([nodes[i], nodes[j]], fill=(0, 180, 255, 60), width=2)
            
    for (x, y) in nodes:
        draw.ellipse([x - 12, y - 12, x + 12, y + 12], fill=(0, 220, 255, 200), outline=(255, 255, 255, 255), width=2)
        
    for d in DIRS:
        splash_img.save(d / "splash.png")
    print("  [+] Generated splash.png (1280x720)")
    
    # 2. Logo Image (512x256)
    logo_img = Image.new("RGBA", (512, 256), (0, 0, 0, 0))
    draw_logo = ImageDraw.Draw(logo_img)
    
    # Draw stylized Primordials emblem
    draw_logo.polygon([(256, 40), (320, 140), (256, 180), (192, 140)], fill=(0, 200, 255, 220), outline=(255, 255, 255, 255), width=3)
    draw_logo.polygon([(256, 60), (300, 130), (256, 160), (212, 130)], fill=(120, 240, 255, 180))
    
    for d in DIRS:
        logo_img.save(d / "logo.png")
    print("  [+] Generated logo.png (512x256)")
    
    # 3. Loading Bar Container (600x32)
    bar_img = Image.new("RGBA", (600, 32), (0, 0, 0, 0))
    draw_bar = ImageDraw.Draw(bar_img)
    draw_bar.rounded_rectangle([0, 0, 600, 32], radius=8, fill=(20, 25, 35, 220), outline=(60, 80, 110, 255), width=2)
    
    for d in DIRS:
        bar_img.save(d / "loading_bar.png")
    print("  [+] Generated loading_bar.png (600x32)")
    
    # 4. Loading Bar Fill (600x32)
    fill_img = Image.new("RGBA", (600, 32), (0, 0, 0, 0))
    draw_fill = ImageDraw.Draw(fill_img)
    draw_fill.rounded_rectangle([2, 2, 598, 30], radius=6, fill=(0, 180, 255, 255), outline=(120, 240, 255, 255), width=1)
    
    for d in DIRS:
        fill_img.save(d / "loading_bar_fill.png")
    print("  [+] Generated loading_bar_fill.png (600x32)")

if __name__ == "__main__":
    generate_assets()
