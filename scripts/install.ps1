param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Preset,

    [string]$Configuration = "",
    [string]$Prefix = ""
)

. "$PSScriptRoot/common.ps1"

Assert-FoundationConfigured -Preset $Preset

$buildDirectory = Get-FoundationBuildDirectory -Preset $Preset
$configurationName = Get-FoundationConfiguration `
    -Preset $Preset `
    -Configuration $Configuration `
    -DefaultConfiguration "Release"
Assert-FoundationConfiguration -Configuration $configurationName
if (-not $Prefix) {
    $Prefix = Join-Path $script:FoundationDistRoot $Preset
}
$Prefix = Resolve-FoundationPath -Path $Prefix

$arguments = @("--install", $buildDirectory, "--prefix", $Prefix)
$arguments += @("--config", $configurationName)

Invoke-FoundationCMake -Arguments $arguments
