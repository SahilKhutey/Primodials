Add-Type -TypeDefinition @'
using System;
using System.Runtime.InteropServices;
public static class Win32 {
    [DllImport("user32.dll")] public static extern bool ShowWindowAsync(IntPtr hWnd, int nCmdShow);
    [DllImport("user32.dll")] public static extern bool SetForegroundWindow(IntPtr hWnd);
    [DllImport("user32.dll")] public static extern bool IsWindowVisible(IntPtr hWnd);
    [DllImport("user32.dll")] public static extern bool IsIconic(IntPtr hWnd);
    [DllImport("user32.dll")] public static extern bool GetWindowRect(IntPtr hWnd, out RECT lpRect);
    [DllImport("user32.dll")] public static extern bool MoveWindow(IntPtr hWnd, int X, int Y, int nWidth, int nHeight, bool bRepaint);
}
public struct RECT { public int Left; public int Top; public int Right; public int Bottom; }
'@

$p = Get-Process -Name PolygonalPrimordials -ErrorAction SilentlyContinue
if (-not $p) {
    Write-Host 'No PolygonalPrimordials process found.'
    exit 0
}

foreach ($proc in $p) {
    Write-Host "=== PID=$($proc.Id) ==="
    Write-Host "Title: '$($proc.MainWindowTitle)'"
    Write-Host "Handle: $($proc.MainWindowHandle)"
    if ($proc.MainWindowHandle -eq 0) {
        Write-Host 'No main window handle.'
        continue
    }
    $vis = [Win32]::IsWindowVisible($proc.MainWindowHandle)
    $icon = [Win32]::IsIconic($proc.MainWindowHandle)
    $rect = New-Object RECT
    [Win32]::GetWindowRect($proc.MainWindowHandle, [ref]$rect) | Out-Null
    Write-Host "Visible: $vis"
    Write-Host "Iconic: $icon"
    Write-Host "Rect: $($rect.Left),$($rect.Top)-$($rect.Right),$($rect.Bottom)"
    Write-Host 'Restoring and moving window to 0,0...'
    [Win32]::ShowWindowAsync($proc.MainWindowHandle, 9) | Out-Null
    [Win32]::MoveWindow($proc.MainWindowHandle, 0, 0, 1600, 1000, $true) | Out-Null
    [Win32]::SetForegroundWindow($proc.MainWindowHandle) | Out-Null
}
