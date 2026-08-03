@echo off
setlocal enabledelayedexpansion

echo ========================================================
echo       Polygonal Primordials - Simulation Launcher
echo ========================================================

cd /d "%~dp0"

echo [1/3] Checking build environment...
if not exist "build" (
    echo Creating build directory...
    mkdir build
)

echo [2/3] Configuring and compiling...
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed!
    pause
    exit /b %errorlevel%
)

cmake --build build --config Release --target PolygonalPrimordials
if %errorlevel% neq 0 (
    echo [ERROR] Build failed!
    pause
    exit /b %errorlevel%
)

echo [3/3] Launching Simulation...
:: Depending on the generator (e.g. MSVC or Ninja/Make), the executable path varies.
if exist "build\bin\PolygonalPrimordials.exe" (
    start "" "build\bin\PolygonalPrimordials.exe"
) else if exist "build\Release\PolygonalPrimordials.exe" (
    start "" "build\Release\PolygonalPrimordials.exe"
) else if exist "build\Games\PolygonalPrimordials\Release\PolygonalPrimordials.exe" (
    start "" "build\Games\PolygonalPrimordials\Release\PolygonalPrimordials.exe"
) else if exist "build\PolygonalPrimordials.exe" (
    start "" "build\PolygonalPrimordials.exe"
) else if exist "build\Games\PolygonalPrimordials\PolygonalPrimordials.exe" (
    start "" "build\Games\PolygonalPrimordials\PolygonalPrimordials.exe"
) else (
    echo [ERROR] Could not find compiled PolygonalPrimordials.exe!
    pause
    exit /b 1
)

echo Launched successfully!
exit /b 0
