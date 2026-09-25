param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Preset,

    [string]$Output = "",
    [int]$Parallel = 0,
    [switch]$Fresh,
    [switch]$Keep,
    [Alias("examples-on")]
    [switch]$ExamplesOn,

    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]]$CMakeArguments
)

. "$PSScriptRoot/common.ps1"

$customOutput = [bool]$Output
if (-not $Output) {
    $Output = Join-Path $script:FoundationDistRoot $Preset
}
$Output = Resolve-FoundationPath -Path $Output

if (-not $Keep -and $customOutput) {
    throw "Custom export paths require -Keep; remove custom destinations explicitly"
}

$configureParameters = @{
    Preset = $Preset
}
$effectiveCMakeArguments = @(
    $CMakeArguments | Where-Object {
        -not [string]::IsNullOrWhiteSpace($_)
    }
)
if ($ExamplesOn) {
    $effectiveCMakeArguments += "-DFOUNDATION_EXAMPLES=ON"
}
else {
    $effectiveCMakeArguments += "-DFOUNDATION_EXAMPLES=OFF"
}
if ($effectiveCMakeArguments.Count -gt 0) {
    $configureParameters.CMakeArguments = $effectiveCMakeArguments
}
if ($Fresh) {
    $configureParameters.Fresh = $true
}
& "$PSScriptRoot/configure.ps1" @configureParameters
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

$buildDirectory = Get-FoundationBuildDirectory -Preset $Preset
$buildArguments = @(
    "--build", $buildDirectory,
    "--config", "Release",
    "--target", "FoundationExportArtifacts"
)
if ($Parallel -gt 0) {
    $buildArguments += @("--parallel", $Parallel.ToString())
}
Invoke-FoundationCMake -Arguments $buildArguments

if (-not $Keep) {
    Assert-FoundationDistChild -Path $Output
    if (Test-Path -LiteralPath $Output) {
        Remove-Item -LiteralPath $Output -Recurse -Force
    }
}

$installParameters = @{
    Preset = $Preset
    Prefix = $Output
}
& "$PSScriptRoot/install.ps1" @installParameters
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

Write-Host "Exported Foundation (Release) to $Output"
