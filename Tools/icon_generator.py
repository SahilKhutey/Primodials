#!/usr/bin/env python3
"""
icon_generator.py

Auto-generates UI icons for Polygonal Primordials using PIL/Pillow.
Creates all needed icons in the correct sizes and styles.

Usage:
    python tools/icon_generator.py
    python tools/icon_generator.py --theme dark
    python tools/icon_generator.py --output assets/ui/icons/
"""

import os
import sys
import argparse
from pathlib import Path

# Icon definitions: (name, unicode_glyph_or_svg, color)
ICONS = {
    # Toolbar icons (32x32)
    'play': ('▶', '#4ade80'),
    'pause': ('❚❚', '#fbbf24'),
    'stop': ('■', '#f87171'),
    'step_forward': ('⏩', '#60a5fa'),
    'step_backward': ('⏪', '#60a5fa'),
    'save': ('💾', '#a78bfa'),
    'load': ('📂', '#a78bfa'),
    'settings': ('⚙', '#94a3b8'),
    'close': ('×', '#f87171'),
    'menu': ('☰', '#e2e8f0'),
    'add': ('+', '#4ade80'),
    'remove': ('−', '#f87171'),
    'edit': ('✎', '#60a5fa'),
    'delete': ('🗑', '#f87171'),
    
    # Theme/Style
    'theme': ('🎨', '#c084fc'),
    'palette': ('🎭', '#c084fc'),
    'brightness': ('☀', '#fbbf24'),
    'contrast': ('◐', '#94a3b8'),
    
    # Audio
    'volume_up': ('🔊', '#4ade80'),
    'volume_down': ('🔉', '#fbbf24'),
    'volume_mute': ('🔇', '#f87171'),
    'music': ('🎵', '#c084fc'),
    
    # Status
    'check': ('✓', '#4ade80'),
    'warning': ('⚠', '#fbbf24'),
    'error': ('✕', '#f87171'),
    'info': ('ℹ', '#60a5fa'),
    'star': ('★', '#fbbf24'),
    'star_outline': ('☆', '#94a3b8'),
    'lock': ('🔒', '#94a3b8'),
    'unlock': ('🔓', '#4ade80'),
    
    # Simulation
    'creature': ('⬡', '#4ade80'),
    'predator': ('◈', '#f87171'),
    'prey': ('○', '#60a5fa'),
    'food': ('●', '#4ade80'),
    'water': ('💧', '#60a5fa'),
    'terrain': ('▲', '#a78bfa'),
    
    # Achievement
    'trophy': ('🏆', '#fbbf24'),
    'medal': ('🏅', '#c0c0c0'),
    'crown': ('👑', '#fbbf24'),
    
    # UI navigation
    'up': ('▲', '#e2e8f0'),
    'down': ('▼', '#e2e8f0'),
    'left': ('◀', '#e2e8f0'),
    'right': ('▶', '#e2e8f0'),
    'home': ('⌂', '#e2e8f0'),
    'back': ('↩', '#e2e8f0'),
    'forward': ('↪', '#e2e8f0'),
    'refresh': ('↻', '#60a5fa'),
    
    # Game-specific
    'dna': ('🧬', '#4ade80'),
    'genome': ('⚛', '#c084fc'),
    'evolution': ('⤴', '#4ade80'),
    'extinction': ('☠', '#f87171'),
    'boom': ('📈', '#4ade80'),
    'bust': ('📉', '#f87171'),
    'ecosystem': ('🌍', '#4ade80'),
    
    # Help/Info
    'help': ('?', '#60a5fa'),
    'docs': ('📖', '#94a3b8'),
    'discord': ('💬', '#5865F2'),
    'github': ('⌨', '#e2e8f0'),
}


def hex_to_rgba(hex_color, alpha=255):
    """Convert hex color to RGBA tuple."""
    hex_color = hex_color.lstrip('#')
    r = int(hex_color[0:2], 16)
    g = int(hex_color[2:4], 16)
    b = int(hex_color[4:6], 16)
    return (r, g, b, alpha)


def generate_icon_pillow(name, glyph, color, size=64):
    """Generate icon using PIL/Pillow."""
    from PIL import Image, ImageDraw, ImageFont, ImageFilter
    img = Image.new('RGBA', (size, size), (0, 0, 0, 0))
    draw = ImageDraw.Draw(img)
    
    font_size = int(size * 0.7)
    font_paths_to_try = [
        'C:/Windows/Fonts/seguiemj.ttf',
        'C:/Windows/Fonts/arial.ttf',
        '/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf',
    ]
    font = None
    for fp in font_paths_to_try:
        if os.path.exists(fp):
            try:
                font = ImageFont.truetype(fp, font_size)
                break
            except Exception:
                pass
    if font is None:
        font = ImageFont.load_default()
    
    rgba = hex_to_rgba(color, 255)
    draw.text((size // 4, size // 4), glyph, font=font, fill=rgba)
    return img


def generate_icon_svg(name, glyph, color, size=64):
    """Fallback generator outputting SVG vector."""
    rgba = hex_to_rgba(color, 255)
    svg_content = f'''<svg xmlns="http://www.w3.org/2000/svg" width="{size}" height="{size}" viewBox="0 0 {size} {size}">
  <rect width="100%" height="100%" fill="none"/>
  <text x="50%" y="55%" dominant-baseline="middle" text-anchor="middle" font-size="{int(size*0.6)}" fill="{color}">{glyph}</text>
</svg>'''
    return svg_content


def generate_icon_set(output_dir, theme='dark', size=64):
    """Generate complete icon set."""
    output_path = Path(output_dir)
    output_path.mkdir(parents=True, exist_ok=True)
    
    has_pil = False
    try:
        from PIL import Image
        has_pil = True
    except ImportError:
        pass
    
    for name, (glyph, color) in ICONS.items():
        try:
            if has_pil:
                icon = generate_icon_pillow(name, glyph, color, size)
                file_path = output_path / f"{name}.png"
                icon.save(file_path)
            else:
                svg_data = generate_icon_svg(name, glyph, color, size)
                file_path = output_path / f"{name}.svg"
                with open(file_path, "w", encoding="utf-8") as f:
                    f.write(svg_data)
            print(f"  [+] Generated: {file_path.name}")
        except Exception as e:
            print(f"  [-] Failed {name}: {e}")
    
    print(f"\n[+] Generated {len(ICONS)} icons in {output_path}")


def generate_theme_thumbnails(themes_dir, output_dir):
    """Generate thumbnail images for each theme."""
    import json
    import math
    
    themes_path = Path(themes_dir)
    output_path = Path(output_dir)
    output_path.mkdir(parents=True, exist_ok=True)
    
    has_pil = False
    try:
        from PIL import Image, ImageDraw
        has_pil = True
    except ImportError:
        pass

    for theme_dir in themes_path.iterdir():
        if not theme_dir.is_dir():
            continue
        
        theme_json = theme_dir / "theme.json"
        if not theme_json.exists():
            continue
        
        try:
            with open(theme_json, 'r', encoding='utf-8') as f:
                theme_data = json.load(f)
            
            thumb_size = 256
            if has_pil:
                img = Image.new('RGB', (thumb_size, thumb_size))
                draw = ImageDraw.Draw(img)
                
                top_color = tuple(int(c * 255) for c in theme_data.get('backgroundTopColor', [0.1, 0.1, 0.2, 1.0])[:3])
                bot_color = tuple(int(c * 255) for c in theme_data.get('backgroundBottomColor', [0.05, 0.05, 0.1, 1.0])[:3])
                
                for y in range(thumb_size):
                    t = y / float(thumb_size)
                    color = tuple(int(top_color[i] * (1.0 - t) + bot_color[i] * t) for i in range(3))
                    draw.line([(0, y), (thumb_size, y)], fill=color)
                
                palette = theme_data.get('speciesPalette', [])
                if palette:
                    for i, color_rgba in enumerate(palette[:8]):
                        color = tuple(int(c * 255) for c in color_rgba[:3])
                        angle = (i / 8.0) * 2.0 * math.pi
                        x = thumb_size // 2 + int(math.cos(angle) * thumb_size * 0.25)
                        y = thumb_size // 2 + int(math.sin(angle) * thumb_size * 0.25)
                        draw.ellipse([x - 12, y - 12, x + 12, y + 12], fill=color)
                
                output_file = output_path / f"{theme_dir.name}.png"
                img.save(output_file)
                # Also save directly into theme directory as preview.png
                img.save(theme_dir / "preview.png")
                print(f"  [+] Generated theme thumbnail: {theme_dir.name}.png & preview.png")
            else:
                print(f"  [-] PIL not installed, skipping theme thumbnail for {theme_dir.name}")
        except Exception as e:
            print(f"  [-] Failed theme thumbnail {theme_dir.name}: {e}")


def main():
    parser = argparse.ArgumentParser(description='Generate UI assets')
    parser.add_argument('--output', default='assets/ui/icons/',
                       help='Output directory')
    parser.add_argument('--size', type=int, default=64,
                       help='Icon size in pixels')
    parser.add_argument('--theme-thumbs', action='store_true',
                       help='Generate theme thumbnails')
    parser.add_argument('--themes-dir', default='Content/Themes',
                       help='Themes directory')
    args = parser.parse_args()
    
    print("=== UI Asset Generator ===\n")
    print("Generating UI icons...")
    generate_icon_set(args.output, size=args.size)
    
    if args.theme_thumbs:
        print("\nGenerating theme thumbnails...")
        thumb_output = Path(args.output) / "themes"
        generate_theme_thumbnails(args.themes_dir, thumb_output)
    
    print("\n[+] Asset generation complete!")


if __name__ == "__main__":
    main()
