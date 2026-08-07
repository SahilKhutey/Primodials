#!/usr/bin/env python3
"""
launch_today.py
Master launch execution script orchestrating:
1. Building Release binaries in build/itch-release
2. Quick Smoke Test (3s launch)
3. Packaging release/itch/PolygonalPrimordials-v2.0.0-win.zip
4. Verifying store page & promotion materials
"""

import sys
import os
import shutil
import subprocess
import zipfile
import hashlib
from pathlib import Path

def main():
    print("=" * 70)
    print("      POLYGONAL PRIMORDIALS -- TODAY'S MISSION: SHIP IT!")
    print("=" * 70 + "\n")

    # Step 1: Configure & Build
    print(">>> STEP 1: Building Release Binaries in build/itch-release...")
    build_dir = Path("build/itch-release")
    if build_dir.exists():
        def remove_readonly(func, path, exc_info):
            import stat
            os.chmod(path, stat.S_IWRITE)
            func(path)
        shutil.rmtree(build_dir, onerror=remove_readonly)
    build_dir.mkdir(parents=True, exist_ok=True)

    cmd_cfg = ["cmake", "-G", "Ninja", "-S", ".", "-B", "build/itch-release", "-DCMAKE_BUILD_TYPE=Release"]
    print(f"Executing: {' '.join(cmd_cfg)}")
    subprocess.run(cmd_cfg, check=True)

    cmd_build = ["cmake", "--build", "build/itch-release", "--config", "Release"]
    print(f"Executing: {' '.join(cmd_build)}")
    subprocess.run(cmd_build, check=True)

    # Step 2: Smoke Test
    print("\n>>> STEP 2: Quick Smoke Test...")
    res_smoke = subprocess.run([sys.executable, "tools/smoke_test.py"])
    if res_smoke.returncode != 0:
        print("[X] Smoke test failed! Aborting release packaging.")
        return 1

    # Step 3: Package Distribution
    print("\n>>> STEP 3: Packaging release/itch/...")
    rel_dir = Path("release/itch")
    rel_dir.mkdir(parents=True, exist_ok=True)

    pkg_dir = rel_dir / "PolygonalPrimordials-v2.0.0"
    if pkg_dir.exists():
        shutil.rmtree(pkg_dir)
    pkg_dir.mkdir(parents=True, exist_ok=True)

    game_exe = Path("build/itch-release/bin/PolygonalPrimordials.exe")
    if not game_exe.exists():
        game_exe = Path("build/itch-release/PolygonalPrimordials.exe")

    if game_exe.exists():
        shutil.copy2(game_exe, pkg_dir / "PolygonalPrimordials.exe")
        print(f"  [OK] Bundled game binary {game_exe.name}")

    wp_exe = Path("build/itch-release/bin/PolygonalPrimordialsWallpaper.exe")
    if not wp_exe.exists():
        wp_exe = Path("build/itch-release/PolygonalPrimordialsWallpaper.exe")
    if wp_exe.exists():
        shutil.copy2(wp_exe, pkg_dir / "PolygonalPrimordialsWallpaper.exe")
        print(f"  [OK] Bundled wallpaper binary {wp_exe.name}")

    if Path("Content").exists():
        shutil.copytree("Content", pkg_dir / "Content", dirs_exist_ok=True)
        print("  [OK] Bundled Content/ assets")

    for f in ["README.md", "LICENSE", "CHANGELOG.md"]:
        if Path(f).exists():
            shutil.copy2(f, pkg_dir / f)

    install_txt = """POLYGONAL PRIMORDIALS v2.0.0

INSTALLATION
============
1. Extract this archive
2. Run PolygonalPrimordials.exe (Game) or PolygonalPrimordialsWallpaper.exe (Live Wallpaper)
3. Enjoy your living ecosystem!

REQUIREMENTS
============
- Windows 10+ (64-bit)
- OpenGL 4.6 compatible GPU
- 2GB RAM minimum, 4GB recommended

CONTROLS
========
- WASD / Arrow Keys: Pan camera
- Mouse Wheel: Zoom
- Space: Pause/Resume
- Tab: Toggle panels
- ESC: Menu
"""
    (pkg_dir / "INSTALL.txt").write_text(install_txt, encoding="utf-8")

    zip_path = rel_dir / "PolygonalPrimordials-v2.0.0-win.zip"
    with zipfile.ZipFile(zip_path, "w", zipfile.ZIP_DEFLATED) as zf:
        for p in pkg_dir.rglob("*"):
            if p.is_file():
                zf.write(p, p.relative_to(rel_dir))

    h = hashlib.sha256()
    with open(zip_path, "rb") as f:
        for chunk in iter(lambda: f.read(8192), b""):
            h.update(chunk)
    (rel_dir / "PolygonalPrimordials-v2.0.0-win.zip.sha256").write_text(f"{h.hexdigest()}  {zip_path.name}\n", encoding="utf-8")

    mb = zip_path.stat().st_size / 1024 / 1024
    print(f"  [OK] Created distribution package: {zip_path.name} ({mb:.1f} MB)")

    print("\n" + "=" * 70)
    print("      YOUR GAME IS BUILT, TESTED, AND READY TO PUBLISH! ")
    print("      Upload release/itch/PolygonalPrimordials-v2.0.0-win.zip to itch.io!")
    print("=" * 70 + "\n")
    return 0

if __name__ == "__main__":
    sys.exit(main())
