$procs = Get-Process -Name PolygonalPrimordials -ErrorAction SilentlyContinue
if (-not $procs) {
    Write-Host 'No PolygonalPrimordials process found.'
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

public struct RECT {
    public int Left;
    public int Top;
    public int Right;
    public int Bottom;
}
'@

foreach ($proc in $procs) {
    Write-Host "=== PID=$($proc.Id) ==="
    Write-Host "Title: '$($proc.MainWindowTitle)'"
    Write-Host "Handle: $($proc.MainWindowHandle)"
    if ($proc.MainWindowHandle -eq 0) {
        Write-Host 'No main window handle available.'
        continue
    }
    $vis = [User32.Win32]::IsWindowVisible($proc.MainWindowHandle)
    $icon = [User32.Win32]::IsIconic($proc.MainWindowHandle)
    $rect = New-Object User32.Win32+RECT
    [User32.Win32]::GetWindowRect($proc.MainWindowHandle, [ref]$rect) | Out-Null
    Write-Host "Visible: $vis"
    Write-Host "Iconic: $icon"
    Write-Host "Rect: $($rect.Left),$($rect.Top)-$($rect.Right),$($rect.Bottom)"
    if (-not $vis -or $icon) {
        Write-Host 'Attempting to restore and foreground window...'
        [User32.Win32]::ShowWindowAsync($proc.MainWindowHandle, 9) | Out-Null
        [User32.Win32]::SetForegroundWindow($proc.MainWindowHandle) | Out-Null
    }
}
