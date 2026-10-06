param(
    [string]$Fqbn = "arduino:avr:uno",
    [string]$Cpstl = ""
)

$ErrorActionPreference = "Stop"

. "$PSScriptRoot/common.ps1"

# The declared Arduino source-mode board is arduino:avr:uno. Other cores are
# not validated here.
if (-not (Get-Command arduino-cli -ErrorAction SilentlyContinue)) {
    throw "arduino-cli not found"
}

$root = $script:FoundationRoot
# CPSTL defaults to FOUNDATION_CPSTL_SOURCE or the sibling ../CPSTL.
if (-not $Cpstl) {
    $Cpstl = if ($env:FOUNDATION_CPSTL_SOURCE) { $env:FOUNDATION_CPSTL_SOURCE } else { Join-Path $root "../CPSTL" }
}
if (-not (Test-Path -LiteralPath (Join-Path $Cpstl "library.properties"))) {
    throw "CPSTL Arduino library not found at $Cpstl"
}
$Cpstl = (Resolve-Path -LiteralPath $Cpstl).Path
$examplesRoot = Join-Path $root "examples/Foundation"
$buildRoot = Join-Path $root ("build/arduino/" + ($Fqbn -replace ":", "_"))
if (Test-Path -LiteralPath $buildRoot) {
    Remove-Item -LiteralPath $buildRoot -Recurse -Force
}

# Compile each sketch against the repository and CPSTL as libraries, exactly as
# an Arduino user who installed both would, with the core's unmodified flags
# (gnu++11 on Arduino AVR).
$sketches = Get-ChildItem -Path $examplesRoot -Recurse -Filter *.ino |
    Where-Object {
        $relative = $_.DirectoryName.Substring($examplesRoot.Length + 1)
        ($relative -split '[\\/]').Count -eq 2
    } |
    Sort-Object FullName

$count = 0
foreach ($sketch in $sketches) {
    $name = $sketch.DirectoryName.Substring($examplesRoot.Length + 1) -replace '\\', '/'
    $log = Join-Path $buildRoot "$name.log"
    New-Item -ItemType Directory -Force -Path (Split-Path -Parent $log) | Out-Null
    Write-Host "== $name ($Fqbn)"

    & arduino-cli compile `
        --fqbn $Fqbn `
        --library $root `
        --library $Cpstl `
        --build-path (Join-Path $buildRoot $name) `
        --warnings default `
        $sketch.DirectoryName *> $log
    $status = $LASTEXITCODE
    Get-Content -LiteralPath $log
    if ($status -ne 0) {
        throw "$name failed to compile"
    }

    # The stock core passes -fpermissive, which demotes real type errors to
    # warnings; any warning in Foundation or its examples fails the gate.
    $rootPattern = [regex]::Escape($root)
    $warnings = Select-String -LiteralPath $log -Pattern "warning:" |
        Where-Object { $_.Line -match $rootPattern -or $_.Line -match [regex]::Escape($root -replace '\\', '/') }
    if ($warnings) {
        throw "$name compiled with Foundation warnings"
    }
    $count++
}

if ($count -eq 0) {
    throw "no Arduino sketches found"
}
Write-Host "All $count Arduino sketches compiled for $Fqbn."
