@echo off
rem build_all.bat - Windows Master Build Entry Point

echo ===============================================================
echo Polygonal Primordials - Master Build System (Windows)
echo ===============================================================

set BUILD_DIR=build\itch-release
if not exist %BUILD_DIR% set BUILD_DIR=build

cmake --build %BUILD_DIR% --config Release -j %NUMBER_OF_PROCESSORS%
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Build failed!
    exit /b %ERRORLEVEL%
)

echo.
echo ===============================================================
echo Running Catch2 Test Suite...
echo ===============================================================
if exist %BUILD_DIR%\bin\ShapeEngineTests.exe (
    %BUILD_DIR%\bin\ShapeEngineTests.exe
) else if exist %BUILD_DIR%\Release\ShapeEngineTests.exe (
    %BUILD_DIR%\Release\ShapeEngineTests.exe
)

echo.
echo [✓] Build and tests complete!
