param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Preset,

    [Parameter(Position = 1)]
    [string[]]$Configuration = @(),

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

$configurations = if ($Configuration.Count -gt 0) {
    $Configuration
}
else {
    @("Debug", "Release")
}
foreach ($configurationName in $configurations) {
    Assert-FoundationConfiguration -Configuration $configurationName
}
if ($ExamplesOn -and "Release" -notin $configurations) {
    throw "-ExamplesOn requires the Release configuration"
}

$configureParameters = @{
    Preset = $Preset
}
$effectiveCMakeArguments = @($CMakeArguments)
if ($ExamplesOn) {
    $effectiveCMakeArguments += "-DFOUNDATION_EXAMPLES=ON"
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
foreach ($configurationName in $configurations) {
    $buildTarget = if ($configurationName -eq "Release") {
        "FoundationExportArtifacts"
    }
    else {
        "Foundation"
    }
    $buildArguments = @(
        "--build", $buildDirectory,
        "--config", $configurationName,
        "--target", $buildTarget
    )
    if ($Parallel -gt 0) {
        $buildArguments += @("--parallel", $Parallel.ToString())
    }
    Invoke-FoundationCMake -Arguments $buildArguments
}

if (-not $Keep) {
    Assert-FoundationDistChild -Path $Output
    if (Test-Path -LiteralPath $Output) {
        Remove-Item -LiteralPath $Output -Recurse -Force
    }
}

foreach ($configurationName in $configurations) {
    $installParameters = @{
        Preset = $Preset
        Configuration = $configurationName
        Prefix = $Output
    }
    & "$PSScriptRoot/install.ps1" @installParameters
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

Write-Host "Exported Foundation ($($configurations -join ', ')) to $Output"
