$WshShell = New-Object -comObject WScript.Shell
$DesktopPath = [Environment]::GetFolderPath("Desktop")

# 1. Create Shortcut for the Game/Simulation
$SimShortcut = $WshShell.CreateShortcut("$DesktopPath\Polygonal Primordials.lnk")
$SimShortcut.TargetPath = "$PSScriptRoot\launch_simulation.bat"
$SimShortcut.WorkingDirectory = "$PSScriptRoot"
$SimShortcut.Description = "Launch Polygonal Primordials Simulation"
$SimShortcut.IconLocation = "shell32.dll, 43"
$SimShortcut.Save()
Write-Host "Desktop shortcut created: $DesktopPath\Polygonal Primordials.lnk"

# 2. Create Shortcut for the Editor
$EdShortcut = $WshShell.CreateShortcut("$DesktopPath\Shape Engine Editor.lnk")
$EdShortcut.TargetPath = "$PSScriptRoot\launch_editor.bat"
$EdShortcut.WorkingDirectory = "$PSScriptRoot"
$EdShortcut.Description = "Launch Shape Engine Editor"
$EdShortcut.IconLocation = "shell32.dll, 71"
$EdShortcut.Save()
Write-Host "Desktop shortcut created: $DesktopPath\Shape Engine Editor.lnk"
