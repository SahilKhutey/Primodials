@echo off
REM build_windows_depot.bat
REM Windows-specific depot builder
echo ============================================================
echo Building Windows Depot for Polygonal Primordials
echo ============================================================
set VERSION=2.0.0
set APP_ID=1234560

if not exist "build\steam" mkdir "build\steam"
if not exist "build\steam\depot_windows" mkdir "build\steam\depot_windows"

if exist "build\bin\PolygonalPrimordialsWallpaper.exe" (
    copy "build\bin\PolygonalPrimordialsWallpaper.exe" "build\steam\depot_windows\"
    echo [OK] Copied PolygonalPrimordialsWallpaper.exe
) else (
    echo [WARNING] Windows binary not found at build\bin\PolygonalPrimordialsWallpaper.exe
)

if exist "steam_appid.txt" copy steam_appid.txt "build\steam\depot_windows\"

echo Windows depot build complete.
exit /b 0
