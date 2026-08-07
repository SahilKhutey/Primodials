#!/usr/bin/env python3
"""
build_depots.py

Build Steam depot structure for Polygonal Primordials.
Creates platform-specific depots ready for Steamworks upload.
"""

import os
import sys
import json
import shutil
import hashlib
import argparse
from pathlib import Path
from datetime import datetime

APP_ID = 1234560
VERSION = "2.0.0"
BUILD_ID = f"build_{datetime.now().strftime('%Y%m%d_%H%M%S')}"

DEPOT_COMMON = 1001
DEPOT_WINDOWS = 1002
DEPOT_LINUX = 1003
DEPOT_MACOS = 1004

BUILD_ROOT = Path("build/steam")
SOURCE_ROOT = Path("build")
COMMON_CONTENT = Path("Content")

def sha256_file(filepath: Path) -> str:
    h = hashlib.sha256()
    with open(filepath, "rb") as f:
        for chunk in iter(lambda: f.read(8192), b""):
            h.update(chunk)
    return h.hexdigest()

def get_file_size_mb(path: Path) -> float:
    if path.is_file():
        return path.stat().st_size / 1024 / 1024
    elif path.is_dir():
        total = sum(f.stat().st_size for f in path.rglob('*') if f.is_file())
        return total / 1024 / 1024
    return 0.0

def build_common_depot():
    print("\n[Building common depot]")
    common_dst = BUILD_ROOT / "depot_common"
    common_dst.mkdir(parents=True, exist_ok=True)
    
    if COMMON_CONTENT.exists():
        dst = common_dst / "Content"
        if dst.exists(): shutil.rmtree(dst)
        shutil.copytree(COMMON_CONTENT, dst, ignore=shutil.ignore_patterns("*.tmp"))
        print("  [OK] Copied Content directory")

    if Path("docs").exists():
        dst = common_dst / "docs"
        if dst.exists(): shutil.rmtree(dst)
        shutil.copytree(Path("docs"), dst)
        print("  [OK] Copied docs directory")

    for f in ["README.md", "LICENSE", "CONTRIBUTING.md", "CHANGELOG.md"]:
        if Path(f).exists():
            shutil.copy2(Path(f), common_dst / f)

    manifest = {
        "depot_id": DEPOT_COMMON,
        "depot_name": "Polygonal Primordials - Common Content",
        "version": VERSION,
        "build_id": BUILD_ID
    }
    with open(common_dst / "depot_manifest.json", "w") as f:
        json.dump(manifest, f, indent=2)

    print(f"  [OK] Common depot size: {get_file_size_mb(common_dst):.1f} MB")
    return common_dst

def build_windows_depot():
    print("\n[Building Windows depot]")
    win_dst = BUILD_ROOT / "depot_windows"
    win_dst.mkdir(parents=True, exist_ok=True)

    exe_src = SOURCE_ROOT / "bin" / "PolygonalPrimordialsWallpaper.exe"
    if not exe_src.exists():
        exe_src = SOURCE_ROOT / "PolygonalPrimordialsWallpaper.exe"

    if exe_src.exists():
        shutil.copy2(exe_src, win_dst / "PolygonalPrimordialsWallpaper.exe")
        print("  [OK] Copied Windows wallpaper executable")
    else:
        print(f"  [WARN] Windows binary not found at {exe_src}")

    if Path("steam_appid.txt").exists():
        shutil.copy2("steam_appid.txt", win_dst / "steam_appid.txt")

    manifest = {
        "depot_id": DEPOT_WINDOWS,
        "depot_name": "Polygonal Primordials - Windows",
        "version": VERSION,
        "build_id": BUILD_ID,
        "platform": "windows"
    }
    with open(win_dst / "depot_manifest.json", "w") as f:
        json.dump(manifest, f, indent=2)

    print(f"  [OK] Windows depot size: {get_file_size_mb(win_dst):.1f} MB")
    return win_dst

def generate_vdf(app_id: int):
    print("\n[Generating Steam VDF files]")
    BUILD_ROOT.mkdir(parents=True, exist_ok=True)
    
    app_build = f'''"appbuild"
{{
    "appid"            "{app_id}"
    "desc"             "Polygonal Primordials v{VERSION} - {BUILD_ID}"
    "buildfolder"      "{BUILD_ROOT.absolute()}"
    "betakey"          ""
    "preview"          "0"
    "local"            ""
    "depot"            "{DEPOT_COMMON}"
    "rowtype"          "preview"
}}'''
    with open(BUILD_ROOT / "app_build.vdf", "w") as f:
        f.write(app_build)
    print("  [OK] Generated app_build.vdf")

    for depot_id, name, folder in [(DEPOT_COMMON, "Common", "depot_common"), (DEPOT_WINDOWS, "Windows", "depot_windows")]:
        vdf = f'''"DepotBuildConfig"
{{
    "DepotID"      "{depot_id}"
    "FileMapping"
    {{
        "LocalPath"     ".\\{folder}\\*"
        "DepotPath"      "."
        "recursive"      "1"
    }}
    "FileExclusion" "*.pdb"
    "FileExclusion" "*.tmp"
}}'''
        with open(BUILD_ROOT / f"depot_build_{depot_id}.vdf", "w") as f:
            f.write(vdf)
        print(f"  [OK] Generated depot_build_{depot_id}.vdf ({name})")

def main():
    parser = argparse.ArgumentParser(description="Build Steam depots")
    parser.add_argument("--version", default=VERSION, help="Version string")
    parser.add_argument("--app-id", type=int, default=APP_ID, help="Steam App ID")
    args = parser.parse_args()

    print(f"\n{'='*70}")
    print(f"  STEAM DEPOT BUILDER -- Polygonal Primordials v{args.version}")
    print(f"{'='*70}")

    build_common_depot()
    build_windows_depot()
    generate_vdf(args.app_id)

    print(f"\n[OK] Depot build complete at {BUILD_ROOT.absolute()}")
    return 0

if __name__ == "__main__":
    sys.exit(main())
