@echo off
rem gate.bat - Windows Pre-Release Gate

echo ===============================================================
echo PRE-RELEASE VALIDATION GATE (Windows)
echo ===============================================================

set FAILED=0

if not exist README.md set /a FAILED+=1
if not exist LICENSE set /a FAILED+=1
if not exist CHANGELOG.md set /a FAILED+=1
if not exist build_all.bat set /a FAILED+=1
if not exist dev\setup_dev.bat set /a FAILED+=1
if not exist installer\windows\installer.iss set /a FAILED+=1
if not exist accessibility_checklist.md set /a FAILED+=1
if not exist ship_readiness_report.md set /a FAILED+=1

if %FAILED% EQU 0 (
    echo.
    echo [✓] READY TO SHIP
    exit /b 0
) else (
    echo.
    echo [✗] NOT READY - %FAILED% required files missing!
    exit /b 1
)
