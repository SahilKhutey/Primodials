@echo off
setlocal enabledelayedexpansion

echo ========================================================
echo       Polygonal Primordials - Editor Launcher
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

cmake --build build --config Release --target ShapeEditorApp
if %errorlevel% neq 0 (
    echo [ERROR] Build failed!
    pause
    exit /b %errorlevel%
)

echo [3/3] Launching Editor...
if exist "build\bin\ShapeEditorApp.exe" (
    start "" "build\bin\ShapeEditorApp.exe"
) else if exist "build\Release\ShapeEditorApp.exe" (
    start "" "build\Release\ShapeEditorApp.exe"
) else if exist "build\Editor\Release\ShapeEditorApp.exe" (
    start "" "build\Editor\Release\ShapeEditorApp.exe"
) else if exist "build\ShapeEditorApp.exe" (
    start "" "build\ShapeEditorApp.exe"
) else if exist "build\Editor\ShapeEditorApp.exe" (
    start "" "build\Editor\ShapeEditorApp.exe"
) else (
    echo [ERROR] Could not find compiled ShapeEditorApp.exe!
    pause
    exit /b 1
)

echo Launched successfully!
exit /b 0
