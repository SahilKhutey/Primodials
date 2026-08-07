@echo off
title Launch Primordials (Web Engine - Bolt)
cd /d "%~dp0"
echo ===============================================================
echo   🧬 Launching Primordials Web Application (Bolt / Vite)
echo ===============================================================
echo.
if not exist node_modules (
    echo [INFO] Installing web dependencies...
    cmd /c npm install
)
echo [INFO] Starting Primordials Web Application...
cmd /c npx vite --open
pause
