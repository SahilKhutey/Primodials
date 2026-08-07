#!/usr/bin/env python3
"""
package_installer.py

Packages the release directory dist-release-2.0.0 into standalone distribution archives:
- dist/Primordials-v2.0.0-Setup.zip
- dist/Primordials-v2.0.0-Windows-Portable.zip
Computes SHA256 checksums for dist/ archives.
"""

import os
import shutil
import zipfile
import hashlib
from pathlib import Path

VERSION = "2.0.0"
SRC_DIR = Path("dist-release-2.0.0")
DIST_DIR = Path("dist")

def compute_sha256(filepath):
    h = hashlib.sha256()
    with open(filepath, "rb") as f:
        while chunk := f.read(65536):
            h.update(chunk)
    return h.hexdigest()

def make_zip(source_dir, output_zip):
    output_zip.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(output_zip, 'w', zipfile.ZIP_DEFLATED) as zf:
        for root, dirs, files in os.walk(source_dir):
            for file in files:
                abs_file = Path(root) / file
                rel_file = abs_file.relative_to(source_dir)
                zf.write(abs_file, arcname=rel_file)
    print(f"[+] Zip package created: {output_zip} ({output_zip.stat().st_size} bytes)")

def main():
    if not SRC_DIR.exists():
        print(f"[-] Source directory {SRC_DIR} not found. Run create_release.bat first.")
        return
    
    DIST_DIR.mkdir(parents=True, exist_ok=True)
    
    zip_setup = DIST_DIR / f"Primordials-v{VERSION}-Setup.zip"
    zip_portable = DIST_DIR / f"Primordials-v{VERSION}-Windows-Portable.zip"
    
    print("=== Packaging Primordials Installer Archives ===")
    make_zip(SRC_DIR, zip_setup)
    make_zip(SRC_DIR, zip_portable)
    
    checksums_file = DIST_DIR / "SHA256SUMS"
    lines = []
    for p in sorted(DIST_DIR.glob("*.zip")):
        digest = compute_sha256(p)
        lines.append(f"{digest}  {p.name}\n")
        
    with open(checksums_file, "w", encoding="utf-8") as f:
        f.writelines(lines)
    print(f"[+] Written installer checksums -> {checksums_file}")

if __name__ == "__main__":
    main()
