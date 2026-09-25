param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Preset,

    [int]$Parallel = 0,
    [switch]$Fresh,
    [switch]$SkipExport,
    [string]$Package = ""
)

. "$PSScriptRoot/common.ps1"

if (-not $SkipExport -and $Package) {
    throw "-Package requires -SkipExport; normal validation exports to dist/$Preset"
}

if (-not $SkipExport) {
    $exportParameters = @{
        Preset = $Preset
    }
    if ($Fresh) {
        $exportParameters.Fresh = $true
    }
    if ($Parallel -gt 0) {
        $exportParameters.Parallel = $Parallel
    }

    & "$PSScriptRoot/export.ps1" @exportParameters
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

if (-not $Package) {
    $Package = Join-Path $script:FoundationDistRoot $Preset
}
$packagePrefix = Resolve-FoundationPath -Path $Package
if (-not (Test-Path -LiteralPath $packagePrefix -PathType Container)) {
    throw "Package prefix not found: $packagePrefix"
}

Assert-FoundationConfigured -Preset $Preset
$foundationBuildDirectory = Get-FoundationBuildDirectory -Preset $Preset
$foundationCache = Join-Path $foundationBuildDirectory "CMakeCache.txt"
$consumerSourceDirectory = Join-Path $script:FoundationRoot "tests/PackageConsumer"
$consumerBuildDirectory = Join-Path `
    (Join-Path $script:FoundationBuildRoot "package-consumer") `
    $Preset

function Get-FoundationCacheValue {
    param([Parameter(Mandatory = $true)][string]$Name)

    $match = Select-String `
        -LiteralPath $foundationCache `
        -Pattern "^${Name}:[^=]*=" | `
        Select-Object -First 1

    if (-not $match) {
        return ""
    }

    return ($match.Line -split "=", 2)[1]
}

$generator = Get-FoundationCacheValue -Name "CMAKE_GENERATOR"
$generatorPlatform = Get-FoundationCacheValue -Name "CMAKE_GENERATOR_PLATFORM"
$cxxCompiler = Get-FoundationCacheValue -Name "CMAKE_CXX_COMPILER"
$toolchainFile = Get-FoundationCacheValue -Name "CMAKE_TOOLCHAIN_FILE"
$osxArchitectures = Get-FoundationCacheValue -Name "CMAKE_OSX_ARCHITECTURES"
$crossCompiling = Get-FoundationCacheValue -Name "CMAKE_CROSSCOMPILING"

if ($crossCompiling -eq "TRUE") {
    throw "Package execution requires a native preset; validate cross-compiled packages in target firmware"
}

if (Test-Path -LiteralPath $consumerBuildDirectory) {
    Remove-Item -LiteralPath $consumerBuildDirectory -Recurse -Force
}

$configureArguments = @(
    "-S", $consumerSourceDirectory,
    "-B", $consumerBuildDirectory,
    "-G", $generator,
    "-DFOUNDATION_PACKAGE_PREFIX=$packagePrefix"
)

if ($generatorPlatform) {
    $configureArguments += @("-A", $generatorPlatform)
}
if ($osxArchitectures) {
    $configureArguments += "-DCMAKE_OSX_ARCHITECTURES=$osxArchitectures"
}
if ($toolchainFile) {
    $configureArguments += "-DCMAKE_TOOLCHAIN_FILE=$toolchainFile"
}
elseif ($cxxCompiler -and $generator -notmatch "^(Visual Studio|Xcode)") {
    $configureArguments += "-DCMAKE_CXX_COMPILER=$cxxCompiler"
}

Invoke-FoundationCMake -Arguments $configureArguments

$buildArguments = @(
    "--build", $consumerBuildDirectory,
    "--config", "Release"
)
if ($Parallel -gt 0) {
    $buildArguments += @("--parallel", $Parallel.ToString())
}
Invoke-FoundationCMake -Arguments $buildArguments

$testArguments = @(
    "--test-dir", $consumerBuildDirectory,
    "--build-config", "Release",
    "--output-on-failure",
    "--no-tests=error"
)
& ctest @testArguments
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

Write-Host "Validated exported Foundation package at $packagePrefix"
