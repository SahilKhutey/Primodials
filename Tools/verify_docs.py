#!/usr/bin/env python3
"""
verify_docs.py
Documentation completeness and internal link checker.
"""

import sys
import re
from pathlib import Path

def main():
    print("=" * 60)
    print("  DOCUMENTATION QUALITY & LINK CHECK")
    print("=" * 60 + "\n")

    docs = [
        "README.md", "LICENSE", "CONTRIBUTING.md", "CHANGELOG.md",
        "docs/index.md", "docs/getting_started.md", "Marketing/press_kit/PRESS_KIT.md"
    ]

    for d in docs:
        p = Path(d)
        if p.exists():
            lines = len(p.read_text(encoding="utf-8", errors="ignore").splitlines())
            print(f"  [OK] {d:35s} ({lines:4d} lines)")
        else:
            print(f"  [WARN] {d:35s} (MISSING)")

    print("\n[Internal Link Audit]")
    broken_links = 0
    for md_file in Path(".").rglob("*.md"):
        if ".git" in md_file.parts or "build" in md_file.parts: continue
        content = md_file.read_text(encoding="utf-8", errors="ignore")
        links = re.findall(r'\[.*?\]\((.*?)\)', content)
        for link in links:
            if link.startswith("http") or link.startswith("#") or link.startswith("file://"): continue
            target = (md_file.parent / link.split("#")[0]).resolve()
            if not target.exists():
                print(f"  [WARN] Broken link in {md_file.name}: {link}")
                broken_links += 1

    if broken_links == 0:
        print("  [OK] All relative markdown links are valid")

    return 0

if __name__ == "__main__":
    sys.exit(main())
