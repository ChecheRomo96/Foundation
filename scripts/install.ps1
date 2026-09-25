param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Preset,

    [string]$Prefix = ""
)

. "$PSScriptRoot/common.ps1"

Assert-FoundationConfigured -Preset $Preset

$buildDirectory = Get-FoundationBuildDirectory -Preset $Preset
if (-not $Prefix) {
    $Prefix = Join-Path $script:FoundationDistRoot $Preset
}
$Prefix = Resolve-FoundationPath -Path $Prefix

$arguments = @("--install", $buildDirectory, "--prefix", $Prefix)
$arguments += @("--config", "Release")

Invoke-FoundationCMake -Arguments $arguments
