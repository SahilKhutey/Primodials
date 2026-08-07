# Install build dependencies for Windows
Set-ExecutionPolicy -ExecutionPolicy Bypass -Scope Process -Force
Write-Host "Installing dependencies for Windows..." -ForegroundColor Cyan

if (Get-Command winget -ErrorAction SilentlyContinue) {
    Write-Host "Using WinGet..."
    winget install --id Kitware.CMake -e --accept-source-agreements --accept-package-agreements
    winget install --id Git.Git -e --accept-source-agreements --accept-package-agreements
    winget install --id Ninja-build.Ninja -e --accept-source-agreements --accept-package-agreements
    winget install --id Python.Python.3.12 -e --accept-source-agreements --accept-package-agreements
} elseif (Get-Command choco -ErrorAction SilentlyContinue) {
    choco install -y cmake git ninja python3
} elseif (Get-Command scoop -ErrorAction SilentlyContinue) {
    scoop install cmake git ninja python
}

Write-Host "✓ Windows dependencies installed" -ForegroundColor Green
