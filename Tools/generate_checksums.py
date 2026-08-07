#!/usr/bin/env python3
"""
generate_checksums.py

Computes SHA256 checksums for all files in dist-release-2.0.0/ and writes SHA256SUMS.
"""

import hashlib
from pathlib import Path

def compute_sha256(filepath):
    h = hashlib.sha256()
    with open(filepath, "rb") as f:
        while chunk := f.read(65536):
            h.update(chunk)
    return h.hexdigest()

def main():
    target_dir = Path("dist-release-2.0.0")
    if not target_dir.exists():
        print(f"[-] Directory {target_dir} does not exist.")
        return
    
    checksums_file = target_dir / "SHA256SUMS"
    lines = []
    
    for p in sorted(target_dir.rglob("*")):
        if p.is_file() and p.name != "SHA256SUMS":
            rel_path = p.relative_to(target_dir)
            digest = compute_sha256(p)
            lines.append(f"{digest}  {rel_path.as_posix()}\n")
    
    with open(checksums_file, "w", encoding="utf-8") as f:
        f.writelines(lines)
    
    print(f"[+] Computed {len(lines)} SHA256 checksums -> {checksums_file}")

if __name__ == "__main__":
    main()
