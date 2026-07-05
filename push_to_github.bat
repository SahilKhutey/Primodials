@echo off
echo ===================================================
echo Pushing Polygonal Primordials to GitHub...
echo ===================================================

cd /d "%~dp0"

echo [1/4] Initializing git repository (if needed)...
git init

echo [2/4] Adding all files to staging...
git add .

echo [3/4] Committing changes...
git commit -m "Complete Phase A-H: Polygonal Primordials Engine and LiveOps"

echo [4/4] Setting remote and pushing to GitHub...
git branch -M main
git remote add origin https://github.com/SahilKhutey/Primodials
git remote set-url origin https://github.com/SahilKhutey/Primodials
git push -u origin main

echo ===================================================
echo Done! Check your repository online.
echo ===================================================
pause
