@echo off
rem setup_dev.bat - Windows One-Command Developer Setup

echo ===============================================================
echo Polygonal Primordials - Developer Setup (Windows)
echo ===============================================================

where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] CMake not found on PATH. Please install CMake.
    exit /b 1
)

where python >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Python not found on PATH. Please install Python.
    exit /b 1
)

if not exist .venv (
    echo Creating Python virtual environment...
    python -m venv .venv
)

if not exist .vscode (
    mkdir .vscode
    if exist dev\vscode\settings.json copy dev\vscode\settings.json .vscode\ >nul
    if exist dev\vscode\tasks.json copy dev\vscode\tasks.json .vscode\ >nul
    if exist dev\vscode\extensions.json copy dev\vscode\extensions.json .vscode\ >nul
)

echo.
echo Running master build...
call build_all.bat

echo.
echo [✓] Windows Developer Setup Complete!
