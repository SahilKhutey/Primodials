@echo off
rem create_release.bat
rem Creates a complete distributable release package on Windows.

set VERSION=2.0.0
set TIMESTAMP=%date:~10,4%%date:~4,2%%date:~7,2%
set RELEASE_NAME=PolygonalPrimordials-v%VERSION%-%TIMESTAMP%
set RELEASE_DIR=dist-release-%VERSION%

echo Creating release package: %RELEASE_NAME%

call build_all.bat
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Build failed! Cannot package.
    exit /b %ERRORLEVEL%
)

if exist %RELEASE_DIR% rmdir /s /q %RELEASE_DIR%
mkdir %RELEASE_DIR%
mkdir %RELEASE_DIR%\binaries\Windows
mkdir %RELEASE_DIR%\installers
mkdir %RELEASE_DIR%\documentation
mkdir %RELEASE_DIR%\Content

echo Copying binaries...
if exist build\itch-release\bin\PolygonalPrimordials.exe copy build\itch-release\bin\PolygonalPrimordials.exe %RELEASE_DIR%\binaries\Windows\
if exist build\itch-release\bin\PolygonalPrimordials.exe copy build\itch-release\bin\PolygonalPrimordials.exe %RELEASE_DIR%\Launch_Primordials.exe
if exist build\itch-release\bin\PolygonalPrimordialsWallpaper.exe copy build\itch-release\bin\PolygonalPrimordialsWallpaper.exe %RELEASE_DIR%\binaries\Windows\
if exist build\itch-release\bin\ShapeEngineTests.exe copy build\itch-release\bin\ShapeEngineTests.exe %RELEASE_DIR%\binaries\Windows\
if exist Launch_Primordials.bat copy Launch_Primordials.bat %RELEASE_DIR%\

echo Copying assets and content...
xcopy /E /I /Y Content %RELEASE_DIR%\Content\

echo Copying documentation...
copy README.md %RELEASE_DIR%\documentation\
copy README_DEV.md %RELEASE_DIR%\documentation\
copy LICENSE %RELEASE_DIR%\documentation\
copy CHANGELOG.md %RELEASE_DIR%\documentation\
copy accessibility_checklist.md %RELEASE_DIR%\documentation\
copy ship_readiness_report.md %RELEASE_DIR%\documentation\
copy SHIP_READINESS.md %RELEASE_DIR%\documentation\
copy SETUP_PROCESS_AND_CHECKLISTS.md %RELEASE_DIR%\documentation\

echo Generating release info...
(
echo Polygonal Primordials v%VERSION% - Windows Release Package
echo ====================================================
echo.
echo CONTENTS:
echo   binaries\Windows\ - Pre-built binaries for Windows 64-bit
echo   Content\          - Themes, Locales, UI Icons ^& Assets
echo   documentation\    - Comprehensive user and developer guides
echo.
echo BUILD DATE: %date% %time%
echo VERSION: %VERSION%
) > %RELEASE_DIR%\README.txt

echo Release package directory structure ready at %RELEASE_DIR%
echo.
echo ===============================================================
echo  ✓ Release package created successfully! (%RELEASE_DIR%)
echo ===============================================================
