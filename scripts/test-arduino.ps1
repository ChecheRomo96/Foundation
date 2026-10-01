param(
    [string]$Fqbn = "arduino:avr:uno"
)

$ErrorActionPreference = "Stop"

. "$PSScriptRoot/common.ps1"

# The declared Arduino source-mode board is arduino:avr:uno. Other cores are
# not validated here.
if (-not (Get-Command arduino-cli -ErrorAction SilentlyContinue)) {
    throw "arduino-cli not found"
}

$root = $script:FoundationRoot
$examplesRoot = Join-Path $root "examples/Foundation"
$buildRoot = Join-Path $root ("build/arduino/" + ($Fqbn -replace ":", "_"))
if (Test-Path -LiteralPath $buildRoot) {
    Remove-Item -LiteralPath $buildRoot -Recurse -Force
}

# Compile each sketch against the repository itself as the library, exactly as
# an Arduino user who copied it into their libraries folder would, with the
# core's unmodified flags (gnu++11 on Arduino AVR).
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
