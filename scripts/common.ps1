$ErrorActionPreference = "Stop"
Set-StrictMode -Version Latest

$script:FoundationRoot = Split-Path -Parent $PSScriptRoot
$script:FoundationBuildRoot = Join-Path $script:FoundationRoot "build"
$script:FoundationDistRoot = Join-Path $script:FoundationRoot "dist"

function Invoke-FoundationCMake {
    param([Parameter(Mandatory = $true)][string[]]$Arguments)

    & cmake @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "cmake failed with exit code $LASTEXITCODE"
    }
}

function Get-FoundationBuildDirectory {
    param([Parameter(Mandatory = $true)][string]$Preset)
    Assert-FoundationPreset -Preset $Preset
    return Join-Path $script:FoundationBuildRoot $Preset
}

function Assert-FoundationPreset {
    param([Parameter(Mandatory = $true)][string]$Preset)

    if ($Preset -notmatch "^[A-Za-z0-9][A-Za-z0-9_.-]*$" -or $Preset.Contains("..")) {
        throw "Invalid preset name: $Preset"
    }
}

function Get-FoundationConfiguration {
    param(
        [Parameter(Mandatory = $true)][string]$Preset,
        [string]$Configuration = "",
        [string]$DefaultConfiguration = "Debug"
    )

    if ($Configuration) {
        return $Configuration
    }

    if ($Preset -eq "documentation") {
        return "Release"
    }

    return $DefaultConfiguration
}

function Assert-FoundationConfiguration {
    param([Parameter(Mandatory = $true)][string]$Configuration)

    if ($Configuration -notin @("Debug", "Release")) {
        throw "Unsupported configuration '$Configuration'; expected Debug or Release"
    }
}

function Assert-FoundationConfigured {
    param([Parameter(Mandatory = $true)][string]$Preset)

    $buildDirectory = Get-FoundationBuildDirectory -Preset $Preset
    $cache = Join-Path $buildDirectory "CMakeCache.txt"
    if (-not (Test-Path -LiteralPath $cache -PathType Leaf)) {
        throw "Preset '$Preset' is not configured; run scripts/configure.ps1 $Preset first"
    }
}

function Resolve-FoundationPath {
    param([Parameter(Mandatory = $true)][string]$Path)

    if ([System.IO.Path]::IsPathRooted($Path)) {
        return [System.IO.Path]::GetFullPath($Path)
    }

    return [System.IO.Path]::GetFullPath((Join-Path $script:FoundationRoot $Path))
}

function Assert-FoundationDistChild {
    param([Parameter(Mandatory = $true)][string]$Path)

    $distRoot = [System.IO.Path]::GetFullPath($script:FoundationDistRoot).TrimEnd(
        [System.IO.Path]::DirectorySeparatorChar,
        [System.IO.Path]::AltDirectorySeparatorChar
    )
    $candidate = [System.IO.Path]::GetFullPath($Path)
    $prefix = $distRoot + [System.IO.Path]::DirectorySeparatorChar

    $comparison = if ([System.IO.Path]::DirectorySeparatorChar -eq "\") {
        [System.StringComparison]::OrdinalIgnoreCase
    }
    else {
        [System.StringComparison]::Ordinal
    }

    if (-not $candidate.StartsWith($prefix, $comparison)) {
        throw "Refusing to remove export path outside ${distRoot}: $candidate"
    }
}

if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    throw "Required command not found: cmake"
}

Set-Location $script:FoundationRoot
