#!/usr/bin/env python3
"""
verify_git.py
Git repository health and status checker.
"""

import sys
import subprocess
from pathlib import Path

def run(cmd):
    try:
        res = subprocess.run(cmd, shell=True, capture_output=True, text=True)
        return res.stdout.strip()
    except:
        return ""

def main():
    print("=" * 60)
    print("  GIT REPOSITORY HEALTH")
    print("=" * 60 + "\n")

    if not Path(".git").is_dir():
        print("  [WARN] Not a git repository")
        return 0

    branch = run("git rev-parse --abbrev-ref HEAD")
    latest = run("git log -1 --oneline")
    commits = run("git rev-list --count HEAD")
    status = run("git status --porcelain")

    print(f"  Branch:       {branch}")
    print(f"  Latest:       {latest}")
    print(f"  Total commits:{commits}")

    if not status:
        print("  Working tree: [OK] Clean (no uncommitted changes)")
    else:
        modified_count = len(status.splitlines())
        print(f"  Working tree: [WARN] {modified_count} untracked/modified files")

    return 0

if __name__ == "__main__":
    sys.exit(main())
