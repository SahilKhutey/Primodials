#!/usr/bin/env python3
"""
validate_theme.py

Validate a theme directory before uploading to Steam Workshop.
Runs pre-publish checks on theme packages.
"""

import argparse
import json
import os
import sys
from pathlib import Path


def validate_theme(theme_dir: Path, strict: bool = False) -> list:
    issues = []
    
    required = ["theme.json", "preview.jpg", "title.png"]
    missing = [f for f in required if not (theme_dir / f).exists()]
    if missing:
        issues.append((
            "BLOCKER", 
            f"Missing required files: {', '.join(missing)}",
            f"Ensure {', '.join(required)} exist in {theme_dir}"
        ))
    
    theme_json = theme_dir / "theme.json"
    if theme_json.exists():
        try:
            with open(theme_json) as f:
                data = json.load(f)
            
            if "id" not in data:
                issues.append(("ERROR", "Missing 'id' field", "Add lowercase-hyphen id"))
            elif " " in data["id"]:
                issues.append(("ERROR", f"ID contains spaces: '{data['id']}'", "Use hyphens instead"))
            
            if "displayName" not in data:
                issues.append(("WARNING", "Missing 'displayName'", "Add human-readable name"))
        except json.JSONDecodeError as e:
            issues.append(("BLOCKER", f"Invalid JSON in theme.json: {e}", "Validate JSON syntax"))
    
    return issues


def main():
    parser = argparse.ArgumentParser(description="Validate a theme for Steam Workshop")
    parser.add_argument("theme_dir", type=Path, help="Path to theme directory")
    parser.add_argument("--strict", action="store_true", help="Strict mode")
    args = parser.parse_args()
    
    if not args.theme_dir.is_dir():
        print(f"Error: {args.theme_dir} is not a directory", file=sys.stderr)
        return 1
    
    print(f"Validating theme: {args.theme_dir}")
    issues = validate_theme(args.theme_dir, strict=args.strict)
    
    errors = warnings = info = 0
    for severity, message, suggestion in issues:
        icon = {"BLOCKER": "[X]", "ERROR": "[X]", "WARNING": "[!]", "INFO": "[i]"}.get(severity, "   ")
        print(f"{icon} [{severity}] {message}")
        if suggestion:
            print(f"     -> {suggestion}")
        
        if severity in ("BLOCKER", "ERROR"): errors += 1
        elif severity == "WARNING": warnings += 1
        else: info += 1
    
    print(f"\nSummary: {errors} errors, {warnings} warnings, {info} info")
    
    has_blockers = any(sev in ("BLOCKER", "ERROR") for sev, _, _ in issues)
    if has_blockers:
        print("[X] Cannot publish - fix errors above")
        return 1
    else:
        print("[OK] Ready to publish!")
        return 0


if __name__ == "__main__":
    sys.exit(main())
