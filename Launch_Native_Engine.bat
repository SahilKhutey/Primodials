@echo off
title Launch Primordials (Native C++ Engine)
cd /d "%~dp0"

echo ===============================================================
echo        🚀 Launching Primordials Native C++ Engine...
echo ===============================================================

if not exist build\itch-release\bin\PolygonalPrimordials.exe (
    echo [INFO] Game binary not found. Building Primordials...
    call build_all.bat game
    if %ERRORLEVEL% NEQ 0 (
        echo [ERROR] Failed to build Primordials.
        pause
        exit /b 1
    )
)

echo Starting Primordials Native...
start "" build\itch-release\bin\PolygonalPrimordials.exe
