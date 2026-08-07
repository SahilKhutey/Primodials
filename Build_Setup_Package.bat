@echo off
rem Build_Setup_Package.bat
rem One-click setup builder for Primordials on Windows

echo ===============================================================
echo        📦 Building Primordials Setup ^& Package...
echo ===============================================================

call create_release.bat
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Failed to build release package.
    pause
    exit /b 1
)

python tools\generate_checksums.py
python tools\package_installer.py

echo.
echo ===============================================================
echo  ✓ Setup package created in dist\ directory!
echo ===============================================================
explorer dist
