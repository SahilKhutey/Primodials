$ErrorActionPreference = "Stop"

$report = [ordered]@{
  capturedAt = (Get-Date).ToUniversalTime().ToString("o")
  computer = Get-CimInstance Win32_ComputerSystem |
    Select-Object Manufacturer, Model, TotalPhysicalMemory
  cpu = Get-CimInstance Win32_Processor |
    Select-Object -First 1 Name, NumberOfCores, NumberOfLogicalProcessors, MaxClockSpeed
  gpu = Get-CimInstance Win32_VideoController |
    Select-Object Name, AdapterRAM, DriverVersion, VideoModeDescription
  os = Get-CimInstance Win32_OperatingSystem |
    Select-Object Caption, Version, OSArchitecture
}

New-Item -ItemType Directory -Force -Path "benchmarks" | Out-Null
$report | ConvertTo-Json -Depth 8 | Set-Content -Encoding UTF8 "benchmarks/hardware-report.json"
Get-Content "benchmarks/hardware-report.json"
