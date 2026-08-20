param([Parameter(Mandatory=$true)][string]$ArtifactPath)

$ErrorActionPreference = "Stop"

if (!(Test-Path $ArtifactPath)) { throw "Missing artifact: $ArtifactPath" }

$item = Get-Item $ArtifactPath
$hash = Get-FileHash -Algorithm SHA256 -Path $ArtifactPath

[ordered]@{
  path = $item.FullName
  sizeBytes = $item.Length
  sha256 = $hash.Hash
  verifiedAt = (Get-Date).ToUniversalTime().ToString("o")
} | ConvertTo-Json | Set-Content -Encoding UTF8 "release/windows-artifact-check.json"

Write-Output "Artifact: $($item.FullName)"
Write-Output "Size: $($item.Length)"
Write-Output "SHA256: $($hash.Hash)"
