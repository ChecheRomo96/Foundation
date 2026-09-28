param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Preset,

    [int]$Parallel = 0,
    [switch]$Fresh,
    [switch]$SkipExport,
    [string]$Package = "",
    [string]$ToolchainRoot = ""
)

. "$PSScriptRoot/common.ps1"

Assert-FoundationPreset -Preset $Preset

switch ($Preset) {
    "stm32g0b1cbt6_armgcc_cortex_m0plus_soft" {
        $consumerName = "Stm32G0B1Consumer"
        $consumerTarget = "FoundationStm32G0B1Consumer"
    }
    default {
        throw "No firmware consumer is defined for preset '$Preset'"
    }
}

if (-not $SkipExport -and $Package) {
    throw "-Package requires -SkipExport"
}

if ($ToolchainRoot) {
    $ToolchainRoot = Resolve-FoundationPath -Path $ToolchainRoot
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
    if ($ToolchainRoot) {
        $exportParameters.CMakeArguments = @(
            "-DFOUNDATION_ARM_TOOLCHAIN_ROOT=$ToolchainRoot"
        )
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
& "$PSScriptRoot/validate-package.ps1" `
    -Preset $Preset `
    -Package $packagePrefix
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

$foundationBuildDirectory = Get-FoundationBuildDirectory -Preset $Preset
$foundationCache = Join-Path $foundationBuildDirectory "CMakeCache.txt"
$buildInfo = Join-Path `
    $packagePrefix `
    "lib/cmake/Foundation/FoundationBuildInfo.cmake"
$consumerSourceDirectory = Join-Path `
    $script:FoundationRoot `
    "tests/$consumerName"
$consumerBuildDirectory = Join-Path `
    (Join-Path $script:FoundationBuildRoot "stm32-consumer") `
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

function Get-FoundationPackageInfoValue {
    param([Parameter(Mandatory = $true)][string]$Name)

    $match = Select-String `
        -LiteralPath $buildInfo `
        -Pattern ('^set\(' + [regex]::Escape($Name) + ' "(.*)"\)$') | `
        Select-Object -First 1
    if (-not $match) {
        throw "Package identity field not found: $Name"
    }
    return $match.Matches[0].Groups[1].Value
}

$generator = Get-FoundationCacheValue -Name "CMAKE_GENERATOR"
$toolchainFile = Get-FoundationCacheValue -Name "CMAKE_TOOLCHAIN_FILE"
$cxxCompiler = Get-FoundationCacheValue -Name "CMAKE_CXX_COMPILER"
$cacheToolchainRoot = Get-FoundationCacheValue `
    -Name "FOUNDATION_ARM_TOOLCHAIN_ROOT"
$systemName = Get-FoundationPackageInfoValue -Name "Foundation_SYSTEM_NAME"

if ($systemName -ne "Generic") {
    throw "STM32 validation requires a cross-compiled preset"
}
if (-not (Test-Path -LiteralPath $toolchainFile -PathType Leaf)) {
    throw "Toolchain file is unavailable: $toolchainFile"
}
if (-not $ToolchainRoot) {
    $ToolchainRoot = $cacheToolchainRoot
}

if (Test-Path -LiteralPath $consumerBuildDirectory) {
    Remove-Item -LiteralPath $consumerBuildDirectory -Recurse -Force
}

$configureArguments = @(
    "-S", $consumerSourceDirectory,
    "-B", $consumerBuildDirectory,
    "-G", $generator,
    "-DCMAKE_TOOLCHAIN_FILE=$toolchainFile",
    "-DFOUNDATION_PACKAGE_PREFIX=$packagePrefix"
)
if ($ToolchainRoot) {
    $configureArguments += `
        "-DFOUNDATION_ARM_TOOLCHAIN_ROOT=$ToolchainRoot"
}
Invoke-FoundationCMake -Arguments $configureArguments

$buildArguments = @(
    "--build", $consumerBuildDirectory,
    "--config", "Release",
    "--target", $consumerTarget
)
if ($Parallel -gt 0) {
    $buildArguments += @("--parallel", $Parallel.ToString())
}
Invoke-FoundationCMake -Arguments $buildArguments

$elf = Get-ChildItem `
    -LiteralPath $consumerBuildDirectory `
    -Filter "$consumerTarget.elf" `
    -File `
    -Recurse | `
    Select-Object -First 1
if (-not $elf) {
    throw "Consumer ELF was not generated"
}

$artifactDirectory = $elf.DirectoryName
$map = Join-Path $artifactDirectory "$consumerTarget.map"
$hex = Join-Path $artifactDirectory "$consumerTarget.hex"
$bin = Join-Path $artifactDirectory "$consumerTarget.bin"
foreach ($artifact in @($map, $hex, $bin)) {
    if (-not (Test-Path -LiteralPath $artifact -PathType Leaf)) {
        throw "Consumer artifact was not generated: $artifact"
    }
}

$compilerDirectory = Split-Path -Parent $cxxCompiler
function Get-ArmTool {
    param([Parameter(Mandatory = $true)][string]$Name)

    foreach ($fileName in @("$Name.exe", $Name)) {
        $candidate = Join-Path $compilerDirectory $fileName
        if (Test-Path -LiteralPath $candidate -PathType Leaf) {
            return $candidate
        }
    }
    throw "Required Arm tool is unavailable beside the compiler: $Name"
}

$objdump = Get-ArmTool -Name "arm-none-eabi-objdump"
$readelf = Get-ArmTool -Name "arm-none-eabi-readelf"
$nm = Get-ArmTool -Name "arm-none-eabi-nm"
$size = Get-ArmTool -Name "arm-none-eabi-size"

$header = & $readelf -h $elf.FullName 2>&1 | Out-String
if ($LASTEXITCODE -ne 0) {
    throw "readelf failed to inspect the ELF`n$header"
}
$attributes = & $readelf -A $elf.FullName 2>&1 | Out-String
if ($LASTEXITCODE -ne 0) {
    throw "readelf failed to inspect Arm attributes`n$attributes"
}
$sections = & $objdump -h $elf.FullName 2>&1 | Out-String
if ($LASTEXITCODE -ne 0) {
    throw "objdump failed to inspect ELF sections`n$sections"
}
$symbols = & $nm -g $elf.FullName 2>&1 | Out-String
if ($LASTEXITCODE -ne 0) {
    throw "nm failed to inspect ELF symbols`n$symbols"
}
$undefined = & $nm -u $elf.FullName 2>&1 | Out-String
if ($LASTEXITCODE -ne 0) {
    throw "nm failed to inspect undefined symbols`n$undefined"
}

Write-Host $header
Write-Host $attributes
Write-Host $sections
& $size $elf.FullName
if ($LASTEXITCODE -ne 0) {
    throw "size failed to inspect the ELF"
}

if ($header -notmatch "Class:\s+ELF32") {
    throw "Consumer is not ELF32"
}
if ($header -notmatch "Machine:\s+ARM") {
    throw "Consumer is not an Arm executable"
}
if ($header -notmatch "soft-float ABI") {
    throw "Consumer does not use the soft-float ABI"
}
if ($attributes -notmatch "Tag_CPU_arch:\s+v6S-M") {
    throw "Consumer is not compiled for Cortex-M0+ / Armv6-M"
}
if ($attributes -match "Tag_ABI_VFP_args:\s+VFP registers") {
    throw "Consumer unexpectedly advertises the hard-float ABI"
}
if ($sections -notmatch `
    "(?m)^\s*\d+\s+\.isr_vector\s+00000040\s+08000000\s") {
    throw "Vector table is not a 64-byte table at 0x08000000"
}
if ($sections -notmatch `
    "(?m)^\s*\d+\s+\.data\s+[0-9a-f]+\s+20000000\s") {
    throw "Initialized data does not begin in SRAM at 0x20000000"
}
if ($undefined.Trim()) {
    throw "Consumer contains undefined symbols:`n$undefined"
}

foreach ($symbol in @(
    "Reset_Handler",
    "FoundationValidationHalt",
    "FoundationValidationState",
    "FoundationValidationPassed",
    "FoundationValidationTotal"
)) {
    if ($symbols -notmatch "(?m)\s[A-Za-z]\s+$([regex]::Escape($symbol))$") {
        throw "Consumer symbol is missing: $symbol"
    }
}

$mapContents = Get-Content -LiteralPath $map -Raw
$foundationArchive = Join-Path $packagePrefix "lib/libFoundation.a"
if (-not $mapContents.Contains($foundationArchive)) {
    throw "Consumer map does not reference the exported Foundation archive"
}
if (-not $mapContents.Contains("Frequency.cpp")) {
    throw "Consumer did not link Foundation Frequency symbols"
}
if (-not $mapContents.Contains("Period.cpp")) {
    throw "Consumer did not link Foundation Period symbols"
}

Write-Host "Validated STM32 firmware linkage for $Preset"
Write-Host "Firmware artifacts: $artifactDirectory"
Write-Host "Target execution remains required; this script does not claim hardware evidence."
