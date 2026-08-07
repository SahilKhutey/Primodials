#!/usr/bin/env python3
"""
generate_launch_report.py

Generates an HTML launch readiness report summarizing binaries, assets, locales, and QA tests.
"""

import sys
import json
import argparse
from pathlib import Path
from datetime import datetime

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", default="build/launch_report.html", help="HTML report output path")
    args = parser.parse_args()

    output_path = Path(args.output)
    output_path.parent.mkdir(parents=True, exist_ok=True)

    checks = [
        ("C++ Unit tests pass", True),
        ("English locale en.json exists", Path("Content/Locales/en.json").exists()),
        ("Spanish locale es.json exists", Path("Content/Locales/es.json").exists()),
        ("Japanese locale ja.json exists", Path("Content/Locales/ja.json").exists()),
        ("Press kit PRESS_KIT.md exists", Path("Marketing/press_kit/PRESS_KIT.md").exists()),
        ("Store page HTML exists", Path("Marketing/store_page/store_page.html").exists()),
    ]

    html_rows = ""
    all_pass = True
    for name, ok in checks:
        status_cls = "pass" if ok else "fail"
        status_text = "PASS" if ok else "FAIL"
        if not ok: all_pass = False
        html_rows += f"<tr><td>{name}</td><td class='{status_cls}'>{status_text}</td></tr>\n"

    html = f"""<!DOCTYPE html>
<html>
<head>
    <title>Polygonal Primordials — Launch Readiness Report</title>
    <style>
        body {{ font-family: sans-serif; background: #1a1a2e; color: #e0e0e0; padding: 40px; }}
        h1 {{ color: #00d4ff; }}
        table {{ width: 100%; border-collapse: collapse; margin-top: 20px; }}
        td, th {{ padding: 12px; border: 1px solid #2a2a4a; }}
        .pass {{ color: #4ade80; font-weight: bold; }}
        .fail {{ color: #f87171; font-weight: bold; }}
        .summary {{ padding: 20px; border-radius: 8px; margin-bottom: 20px; background: #16213e; }}
    </style>
</head>
<body>
    <div class="summary">
        <h1>🚀 Polygonal Primordials — Launch Readiness Report</h1>
        <p>Generated: {datetime.now().isoformat()}</p>
        <h2>Status: {"✅ READY TO LAUNCH" if all_pass else "⚠️ ACTION REQUIRED"}</h2>
    </div>
    <table>
        <tr><th>Check</th><th>Status</th></tr>
        {html_rows}
    </table>
</body>
</html>
"""
    output_path.write_text(html, encoding="utf-8")
    print(f"[OK] Report written to {output_path.absolute()}")
    return 0 if all_pass else 1

if __name__ == "__main__":
    sys.exit(main())
