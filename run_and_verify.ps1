$exePath = Join-Path (Get-Location) 'build\bin\PolygonalPrimordials.exe'
if (-not (Test-Path $exePath)) {
    Write-Host "Executable not found: $exePath"
    exit 1
}

Write-Host "Starting: $exePath"
$proc = Start-Process -FilePath $exePath -WorkingDirectory (Get-Location) -PassThru
Start-Sleep -Seconds 5
Write-Host "Process started with PID: $($proc.Id)"
Write-Host "HasExited: $($proc.HasExited)"
if ($proc.HasExited) {
    Write-Host "ExitCode: $($proc.ExitCode)"
    exit 0
}

$p = Get-Process -Id $proc.Id -ErrorAction SilentlyContinue
if (-not $p) {
    Write-Host "Process disappeared"
    exit 0
}

Add-Type -Namespace User32 -Name Win32 -MemberDefinition @'
using System;
using System.Runtime.InteropServices;
public class Win32 {
    [DllImport("user32.dll")]
    public static extern bool ShowWindowAsync(IntPtr hWnd, int nCmdShow);
    [DllImport("user32.dll")]
    public static extern bool SetForegroundWindow(IntPtr hWnd);
    [DllImport("user32.dll")]
    public static extern bool IsWindowVisible(IntPtr hWnd);
    [DllImport("user32.dll")]
    public static extern bool IsIconic(IntPtr hWnd);
    [DllImport("user32.dll")]
    public static extern bool GetWindowRect(IntPtr hWnd, out RECT lpRect);
}
public struct RECT { public int Left; public int Top; public int Right; public int Bottom; }
'@

Write-Host "Process info:"
$p | Select-Object Id,ProcessName,MainWindowTitle,MainWindowHandle | Format-List

if ($p.MainWindowHandle -eq 0) {
    Write-Host "MainWindowHandle is 0. There is no visible window handle yet."
    exit 0
}

$h = $p.MainWindowHandle
$visible = [User32.Win32]::IsWindowVisible($h)
$iconic = [User32.Win32]::IsIconic($h)
$rect = New-Object User32.Win32+RECT
[User32.Win32]::GetWindowRect($h, [ref]$rect) | Out-Null
Write-Host "Visible: $visible"
Write-Host "Iconic: $iconic"
Write-Host "Rect: $($rect.Left),$($rect.Top)-$($rect.Right),$($rect.Bottom)"

Write-Host "Attempting to restore and foreground window"
[User32.Win32]::ShowWindowAsync($h, 9) | Out-Null
[User32.Win32]::SetForegroundWindow($h) | Out-Null
Write-Host "Done"
