param(
  [Parameter(Mandatory=$true)][string]$Target,
  [Parameter(Mandatory=$true)][string]$Channel,
  [Parameter(Mandatory=$true)][string]$BuildPath
)

$ErrorActionPreference = "Stop"
$env:ITCH_TARGET = $Target
node Tools/publish-itch.mjs $Channel $BuildPath
