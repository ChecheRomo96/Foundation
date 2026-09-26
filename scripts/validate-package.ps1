param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Preset,

    [string]$Package = ""
)

. "$PSScriptRoot/common.ps1"

Assert-FoundationConfigured -Preset $Preset

if (-not $Package) {
    $Package = Join-Path $script:FoundationDistRoot $Preset
}
$packagePrefix = Resolve-FoundationPath -Path $Package
if (-not (Test-Path -LiteralPath $packagePrefix -PathType Container)) {
    throw "Package prefix not found: $packagePrefix"
}

$buildDirectory = Get-FoundationBuildDirectory -Preset $Preset
$buildInfo = Join-Path `
    $packagePrefix `
    "lib/cmake/Foundation/FoundationBuildInfo.cmake"

Invoke-FoundationCMake -Arguments @(
    "-DPRESET=$Preset",
    "-DBUILD_DIR=$buildDirectory",
    "-DPACKAGE_PREFIX=$packagePrefix",
    "-P", (Join-Path $script:FoundationRoot "tests/cmake/PackageIdentity.cmake")
)

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

$systemName = Get-FoundationPackageInfoValue -Name "Foundation_SYSTEM_NAME"
$systemProcessor = Get-FoundationPackageInfoValue `
    -Name "Foundation_SYSTEM_PROCESSOR"
if ($systemName -ne "Windows") {
    throw "PowerShell package validation expected Windows, got '$systemName'"
}

$archive = Join-Path $packagePrefix "lib/Foundation.lib"
$dumpbin = Get-Command "dumpbin.exe" -ErrorAction SilentlyContinue
if (-not $dumpbin) {
    $vswhere = Join-Path `
        ${env:ProgramFiles(x86)} `
        "Microsoft Visual Studio/Installer/vswhere.exe"
    if (-not (Test-Path -LiteralPath $vswhere -PathType Leaf)) {
        throw "dumpbin.exe and vswhere.exe are unavailable"
    }

    $visualStudioRoot = & $vswhere `
        -latest `
        -products * `
        -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
        -property installationPath
    if ($LASTEXITCODE -ne 0 -or -not $visualStudioRoot) {
        throw "Unable to locate a Visual Studio C++ installation"
    }

    $toolsetsRoot = Join-Path $visualStudioRoot "VC/Tools/MSVC"
    $dumpbin = Get-ChildItem `
        -LiteralPath $toolsetsRoot `
        -Directory | `
        Sort-Object -Property Name -Descending | `
        ForEach-Object {
            $candidate = Join-Path $_.FullName "bin/Hostx64/x64/dumpbin.exe"
            if (Test-Path -LiteralPath $candidate -PathType Leaf) {
                Get-Item -LiteralPath $candidate
            }
        } | `
        Select-Object -First 1
}
if (-not $dumpbin) {
    throw "Unable to locate dumpbin.exe"
}

$dumpbinPath = if ($dumpbin.PSObject.Properties.Name -contains "Source" -and
                   $dumpbin.Source) {
    $dumpbin.Source
}
else {
    $dumpbin.FullName
}
$inspection = & $dumpbinPath /headers $archive 2>&1 | Out-String
if ($LASTEXITCODE -ne 0) {
    throw "dumpbin failed to inspect $archive`n$inspection"
}
Write-Host $inspection

switch -Regex ($systemProcessor) {
    '^(AMD64|amd64|x86_64)$' {
        if ($inspection -notmatch '(?im)^\s*8664 machine \(x64\)') {
            throw "Archive is not Windows x64"
        }
    }
    '^ARM64$' {
        if ($inspection -notmatch '(?im)^\s*AA64 machine \(ARM64\)') {
            throw "Archive is not Windows Arm64"
        }
    }
    default {
        throw "Unsupported Windows processor identity: $systemProcessor"
    }
}

Write-Host "Validated COFF archive architecture: $systemProcessor"
Write-Host "Validated Foundation package identity at $packagePrefix"
