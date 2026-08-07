@echo off
rem Launch_Primordials.bat
rem One-click launcher for Primordials on Windows

echo ===============================================================
echo                🚀 Launching Primordials...
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

echo Starting Primordials...
start "" build\itch-release\bin\PolygonalPrimordials.exe
