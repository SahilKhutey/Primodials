@echo off
rem run_everything.bat - Single Windows command to validate everything

echo ===============================================================
echo Polygonal Primordials - Complete Validation (Windows)
echo ===============================================================

call gate.bat
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Pre-release gate failed!
    exit /b %ERRORLEVEL%
)

call build_all.bat
if %ERRORLEVEL% NEQ 0 (
    echo [ERROR] Build or test suite failed!
    exit /b %ERRORLEVEL%
)

echo.
echo ===============================================================
echo ✅ EVERYTHING PASSED! PACKAGE IS PRODUCTION-READY 🚀
echo ===============================================================
