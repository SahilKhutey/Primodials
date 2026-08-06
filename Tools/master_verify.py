#!/usr/bin/env python3
"""
master_verify.py
Master verification script checking 10 project categories.
"""

import os
import sys
import json
import re
from pathlib import Path
from datetime import datetime

class MasterVerifier:
    def __init__(self):
        self.passed = 0
        self.failed = 0
        self.warnings = 0
        self.critical_fails = []
        self.warning_list = []

    def section(self, title):
        print("\n" + "=" * 60)
        print(f"  {title}")
        print("=" * 60)

    def check(self, category, severity, name, condition):
        ok = bool(condition)
        if ok:
            if severity != "info":
                print(f"  [OK] [{category}] {name}")
                self.passed += 1
        else:
            if severity == "critical":
                print(f"  [X] [{category}] {name}")
                self.failed += 1
                self.critical_fails.append(f"{category}: {name}")
            elif severity == "warning":
                print(f"  [WARN] [{category}] {name}")
                self.warnings += 1
                self.warning_list.append(f"{category}: {name}")

    def run(self):
        print("=" * 65)
        print(f"  POLYGONAL PRIMORDIALS -- MASTER VERIFICATION")
        print(f"  Time: {datetime.now().isoformat()}")
        print("=" * 65)

        # 1. Structure
        self.section("1. PROJECT STRUCTURE")
        self.check("STRUCTURE", "critical", "CMakeLists.txt exists", Path("CMakeLists.txt").exists())
        self.check("STRUCTURE", "critical", "Engine/ directory exists", Path("Engine").is_dir())
        self.check("STRUCTURE", "critical", "Content/ directory exists", Path("Content").is_dir())
        self.check("STRUCTURE", "critical", "docs/ directory exists", Path("docs").is_dir())
        self.check("STRUCTURE", "warning", "tools/ directory exists", Path("tools").is_dir())
        self.check("STRUCTURE", "warning", "EngineTests/ directory exists", Path("EngineTests").is_dir())
        self.check("STRUCTURE", "warning", "Marketing/ directory exists", Path("Marketing").is_dir())
        self.check("STRUCTURE", "warning", "packaging/ directory exists", Path("packaging").is_dir())
        self.check("STRUCTURE", "info", "Git repository initialized", Path(".git").is_dir())
        self.check("STRUCTURE", "info", ".gitignore exists", Path(".gitignore").exists())

        cpp_files = list(Path(".").rglob("*.cpp"))
        h_files = list(Path(".").rglob("*.h*"))
        print(f"  [INFO] Source files: {len(cpp_files)} .cpp, {len(h_files)} headers")

        # 2. Source Code
        self.section("2. SOURCE CODE")
        self.check("CODE", "critical", "Build cache exists", Path("build/CMakeCache.txt").exists())
        
        # Check raw new/delete
        raw_news = 0
        for p in Path("Engine").rglob("*.cpp"):
            text = p.read_text(encoding="utf-8", errors="ignore")
            raw_news += len(re.findall(r'\bnew\b|\bdelete\b', text))
        print(f"  [INFO] Raw memory allocations detected: {raw_news}")

        # 3. Build System
        self.section("3. BUILD SYSTEM")
        cmake_txt = Path("CMakeLists.txt").read_text(encoding="utf-8", errors="ignore") if Path("CMakeLists.txt").exists() else ""
        self.check("BUILD", "critical", "Has project definition", "project(" in cmake_txt)
        self.check("BUILD", "warning", "Has cmake_minimum_required", "cmake_minimum_required" in cmake_txt)
        self.check("BUILD", "warning", "Specifies C++23 standard", "23" in cmake_txt)
        self.check("BUILD", "info", "Build directory exists", Path("build").is_dir())

        # 4. Content Assets
        self.section("4. CONTENT ASSETS")
        self.check("CONTENT", "critical", "Content/Themes directory exists", Path("Content/Themes").is_dir())
        self.check("CONTENT", "critical", "Deep Sea theme present", Path("Content/Themes/deep-sea/theme.json").exists())
        self.check("CONTENT", "warning", "Coral Reef theme present", Path("Content/Themes/coral-reef/theme.json").exists())
        self.check("CONTENT", "warning", "Aurora theme present", Path("Content/Themes/aurora/theme.json").exists())

        # 5. Documentation
        self.section("5. DOCUMENTATION")
        self.check("DOCS", "critical", "README.md exists", Path("README.md").exists())
        self.check("DOCS", "critical", "LICENSE exists", Path("LICENSE").exists())
        self.check("DOCS", "warning", "CONTRIBUTING.md exists", Path("CONTRIBUTING.md").exists())
        self.check("DOCS", "warning", "CHANGELOG.md exists", Path("CHANGELOG.md").exists())
        self.check("DOCS", "warning", "docs/index.md exists", Path("docs/index.md").exists())

        # 6. Marketing Assets
        self.section("6. MARKETING ASSETS")
        self.check("MARKETING", "critical", "Marketing directory exists", Path("Marketing").is_dir())
        screenshots = list(Path("Marketing").rglob("*.png")) + list(Path("Marketing").rglob("*.jpg"))
        self.check("MARKETING", "warning", "At least 3 screenshots/images exist", len(screenshots) >= 3)
        self.check("MARKETING", "warning", "Press kit exists", Path("Marketing/press_kit/PRESS_KIT.md").exists())

        # 7. Localization
        self.section("7. LOCALIZATION")
        self.check("LOCALE", "critical", "Content/Locales directory exists", Path("Content/Locales").is_dir())
        for loc in ["en", "es", "ja", "de", "fr", "zh-CN", "zh-TW", "pt-BR", "ru", "ko", "it"]:
            loc_path = Path(f"Content/Locales/{loc}.json")
            if loc in ["en", "es", "ja"]:
                self.check("LOCALE", "critical", f"{loc}.json exists", loc_path.exists())
            else:
                self.check("LOCALE", "warning", f"{loc}.json exists", loc_path.exists())

        # 8. Legal
        self.section("8. LEGAL & LICENSING")
        self.check("LEGAL", "critical", "LICENSE file present", Path("LICENSE").exists())

        # 9. Deployment
        self.section("9. DEPLOYMENT READINESS")
        game_exe = Path("build/bin/PolygonalPrimordials.exe").exists() or Path("build/PolygonalPrimordials.exe").exists()
        wp_exe = Path("build/bin/PolygonalPrimordialsWallpaper.exe").exists() or Path("build/PolygonalPrimordialsWallpaper.exe").exists()
        self.check("DEPLOY", "critical", "Game binary built", game_exe)
        self.check("DEPLOY", "warning", "Wallpaper binary built", wp_exe)

        # 10. Summary
        self.section("10. VERIFICATION SUMMARY")
        print(f"\n  [OK] Passed:   {self.passed}")
        print(f"  [WARN] Warnings: {self.warnings}")
        print(f"  [X] Failed:   {self.failed}")

        if self.failed == 0:
            print("\n  ============================================")
            print("        READY FOR LAUNCH")
            print("  ============================================")
            return 0
        else:
            print("\n  ============================================")
            print("        NOT READY FOR LAUNCH")
            print("  ============================================")
            for f in self.critical_fails:
                print(f"    - {f}")
            return 1

if __name__ == "__main__":
    verifier = MasterVerifier()
    sys.exit(verifier.run())
