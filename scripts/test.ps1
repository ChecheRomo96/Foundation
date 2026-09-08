param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Preset,

    [string]$Configuration = "",
    [int]$Parallel = 0,
    [string]$Filter = "",
    [switch]$Fresh,
    [switch]$AllowNoTests
)

. "$PSScriptRoot/common.ps1"

$configurationName = Get-FoundationConfiguration `
    -Preset $Preset `
    -Configuration $Configuration `
    -DefaultConfiguration "Debug"
Assert-FoundationConfiguration -Configuration $configurationName

$configureParameters = @{
    Preset = $Preset
    CMakeArguments = @(
        "-DFOUNDATION_TESTING=ON"
    )
}
if ($Fresh) {
    $configureParameters.Fresh = $true
}
& "$PSScriptRoot/configure.ps1" @configureParameters
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

$buildDirectory = Get-FoundationBuildDirectory -Preset $Preset
$buildArguments = @("--build", $buildDirectory, "--config", $configurationName)
if ($Parallel -gt 0) {
    $buildArguments += @("--parallel", $Parallel.ToString())
}
Invoke-FoundationCMake -Arguments $buildArguments

$arguments = @("--test-dir", $buildDirectory, "--output-on-failure")
if (-not $AllowNoTests) {
    $arguments += "--no-tests=error"
}
$arguments += @("--build-config", $configurationName)
if ($Parallel -gt 0) {
    $arguments += @("--parallel", $Parallel.ToString())
}
if ($Filter) {
    $arguments += @("--tests-regex", $Filter)
}

& ctest @arguments
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}
