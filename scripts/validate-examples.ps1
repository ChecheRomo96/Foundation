param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Preset,

    [int]$Parallel = 0,
    [switch]$Fresh
)

. "$PSScriptRoot/common.ps1"

Assert-FoundationPreset -Preset $Preset

$buildDirectory = Get-FoundationBuildDirectory -Preset $Preset
$distDirectory = Join-Path $script:FoundationDistRoot $Preset
$debugDirectory = Join-Path $buildDirectory "bin/Debug"
$releaseDirectory = Join-Path $buildDirectory "bin/Release"
$validationDirectory = Join-Path $buildDirectory "example-validation"
$exportedDirectory = Join-Path $distDirectory "bin/examples"

$debugBuildParameters = @{
    Preset = $Preset
    Configuration = "Debug"
    Target = "FoundationExamples"
    ExamplesOn = $true
}
if ($Fresh) {
    $debugBuildParameters.Fresh = $true
}
if ($Parallel -gt 0) {
    $debugBuildParameters.Parallel = $Parallel
}
& "$PSScriptRoot/build.ps1" @debugBuildParameters
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

$releaseBuildParameters = @{
    Preset = $Preset
    Configuration = "Release"
    Target = "FoundationExamples"
    ExamplesOn = $true
}
if ($Parallel -gt 0) {
    $releaseBuildParameters.Parallel = $Parallel
}
& "$PSScriptRoot/build.ps1" @releaseBuildParameters
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

$exampleRoot = Join-Path $script:FoundationRoot "examples/Foundation"
$expectedDefinitions = @(
    Get-ChildItem -LiteralPath $exampleRoot -Filter "CMakeLists.txt" -File -Recurse |
        Where-Object { $_.Directory.Name -eq "Desktop" }
)
$expectedCount = $expectedDefinitions.Count
if ($expectedCount -eq 0) {
    throw "No desktop example definitions were found"
}
if (-not (Test-Path -LiteralPath $debugDirectory -PathType Container)) {
    throw "Debug example directory not found: $debugDirectory"
}
if (-not (Test-Path -LiteralPath $releaseDirectory -PathType Container)) {
    throw "Release example directory not found: $releaseDirectory"
}

if (Test-Path -LiteralPath $validationDirectory) {
    Remove-Item -LiteralPath $validationDirectory -Recurse -Force
}
foreach ($configuration in @("Debug", "Release", "Export", "Launcher")) {
    New-Item `
        -ItemType Directory `
        -Path (Join-Path $validationDirectory $configuration) `
        -Force | Out-Null
}

function Invoke-FoundationExample {
    param(
        [Parameter(Mandatory = $true)][string]$Executable,
        [Parameter(Mandatory = $true)][string]$StandardOutput,
        [Parameter(Mandatory = $true)][string]$StandardError
    )

    $process = Start-Process `
        -FilePath $Executable `
        -NoNewWindow `
        -Wait `
        -PassThru `
        -RedirectStandardOutput $StandardOutput `
        -RedirectStandardError $StandardError
    if ($process.ExitCode -ne 0) {
        $stdout = Get-Content -LiteralPath $StandardOutput -Raw -ErrorAction SilentlyContinue
        $stderr = Get-Content -LiteralPath $StandardError -Raw -ErrorAction SilentlyContinue
        throw "Example failed with exit code $($process.ExitCode): $Executable`n$stdout`n$stderr"
    }
}

function Assert-FoundationFilesEqual {
    param(
        [Parameter(Mandatory = $true)][string]$Expected,
        [Parameter(Mandatory = $true)][string]$Actual,
        [Parameter(Mandatory = $true)][string]$Description
    )

    $expectedHash = (Get-FileHash -LiteralPath $Expected -Algorithm SHA256).Hash
    $actualHash = (Get-FileHash -LiteralPath $Actual -Algorithm SHA256).Hash
    if ($expectedHash -ne $actualHash) {
        throw "$Description differs"
    }
}

$debugExamples = @(
    Get-ChildItem -LiteralPath $debugDirectory -Filter "Foundation_*_d.exe" -File |
        Sort-Object -Property Name
)
if ($debugExamples.Count -ne $expectedCount) {
    throw "Expected $expectedCount Debug examples, found $($debugExamples.Count)"
}

$releaseExamples = @(
    Get-ChildItem -LiteralPath $releaseDirectory -Filter "Foundation_*.exe" -File |
        Where-Object { $_.Name -notmatch "_d\.exe$" } |
        Sort-Object -Property Name
)
if ($releaseExamples.Count -ne $expectedCount) {
    throw "Expected $expectedCount Release examples, found $($releaseExamples.Count)"
}

foreach ($releaseExample in $releaseExamples) {
    $baseName = [System.IO.Path]::GetFileNameWithoutExtension($releaseExample.Name)
    $debugExample = Join-Path $debugDirectory "${baseName}_d.exe"
    if (-not (Test-Path -LiteralPath $debugExample -PathType Leaf)) {
        throw "Matching Debug example not found: $debugExample"
    }

    $debugStdout = Join-Path $validationDirectory "Debug/${baseName}.stdout.txt"
    $debugStderr = Join-Path $validationDirectory "Debug/${baseName}.stderr.txt"
    $releaseStdout = Join-Path $validationDirectory "Release/${baseName}.stdout.txt"
    $releaseStderr = Join-Path $validationDirectory "Release/${baseName}.stderr.txt"

    Invoke-FoundationExample `
        -Executable $debugExample `
        -StandardOutput $debugStdout `
        -StandardError $debugStderr
    Invoke-FoundationExample `
        -Executable $releaseExample.FullName `
        -StandardOutput $releaseStdout `
        -StandardError $releaseStderr
    Assert-FoundationFilesEqual `
        -Expected $debugStdout `
        -Actual $releaseStdout `
        -Description "$baseName Debug/Release stdout"
    Assert-FoundationFilesEqual `
        -Expected $debugStderr `
        -Actual $releaseStderr `
        -Description "$baseName Debug/Release stderr"
}

$exportParameters = @{
    Preset = $Preset
    ExamplesOn = $true
}
if ($Parallel -gt 0) {
    $exportParameters.Parallel = $Parallel
}
& "$PSScriptRoot/export.ps1" @exportParameters
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}
& "$PSScriptRoot/validate-package.ps1" $Preset
if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

if (-not (Test-Path -LiteralPath $exportedDirectory -PathType Container)) {
    throw "Exported example directory not found: $exportedDirectory"
}

$exportedExamples = @(
    Get-ChildItem -LiteralPath $exportedDirectory -Filter "Foundation_*.exe" -File |
        Sort-Object -Property Name
)
if ($exportedExamples.Count -ne $expectedCount) {
    throw "Expected $expectedCount exported examples, found $($exportedExamples.Count)"
}
if ($exportedExamples | Where-Object { $_.Name -match "_d\.exe$" }) {
    throw "Debug example was exported"
}

$launcher = Join-Path $exportedDirectory "FoundationExamples.cmd"
if (-not (Test-Path -LiteralPath $launcher -PathType Leaf)) {
    throw "Windows example launcher not found: $launcher"
}

$expectedExportNames = @($releaseExamples.Name) + "FoundationExamples.cmd"
$actualExportNames = @(
    Get-ChildItem -LiteralPath $exportedDirectory -File |
        Select-Object -ExpandProperty Name
)
$nameDifference = Compare-Object `
    -ReferenceObject @($expectedExportNames | Sort-Object) `
    -DifferenceObject @($actualExportNames | Sort-Object)
if ($nameDifference) {
    throw "Exported example directory contains missing or unexpected files:`n$($nameDifference | Out-String)"
}

foreach ($releaseExample in $releaseExamples) {
    $baseName = [System.IO.Path]::GetFileNameWithoutExtension($releaseExample.Name)
    $exportedExample = Join-Path $exportedDirectory $releaseExample.Name
    if (-not (Test-Path -LiteralPath $exportedExample -PathType Leaf)) {
        throw "Exported example not found: $exportedExample"
    }

    $exportStdout = Join-Path $validationDirectory "Export/${baseName}.stdout.txt"
    $exportStderr = Join-Path $validationDirectory "Export/${baseName}.stderr.txt"
    Invoke-FoundationExample `
        -Executable $exportedExample `
        -StandardOutput $exportStdout `
        -StandardError $exportStderr
    Assert-FoundationFilesEqual `
        -Expected (Join-Path $validationDirectory "Release/${baseName}.stdout.txt") `
        -Actual $exportStdout `
        -Description "$baseName build/export stdout"
    Assert-FoundationFilesEqual `
        -Expected (Join-Path $validationDirectory "Release/${baseName}.stderr.txt") `
        -Actual $exportStderr `
        -Description "$baseName build/export stderr"
}

function Invoke-FoundationLauncher {
    param(
        [Parameter(Mandatory = $true)][string[]]$InputLines,
        [Parameter(Mandatory = $true)][string]$Name
    )

    $inputPath = Join-Path $validationDirectory "Launcher/${Name}.input.txt"
    $wrapperPath = Join-Path $validationDirectory "Launcher/${Name}.cmd"
    $stdoutPath = Join-Path $validationDirectory "Launcher/${Name}.stdout.txt"
    $stderrPath = Join-Path $validationDirectory "Launcher/${Name}.stderr.txt"

    [System.IO.File]::WriteAllLines($inputPath, $InputLines)
    @(
        "@echo off",
        "call `"$launcher`" < `"$inputPath`"",
        "exit /b %ERRORLEVEL%"
    ) | Set-Content -LiteralPath $wrapperPath -Encoding ascii

    $process = Start-Process `
        -FilePath "cmd.exe" `
        -ArgumentList @("/d", "/c", "`"$wrapperPath`"") `
        -NoNewWindow `
        -Wait `
        -PassThru `
        -RedirectStandardOutput $stdoutPath `
        -RedirectStandardError $stderrPath
    if ($process.ExitCode -ne 0) {
        $stdout = Get-Content -LiteralPath $stdoutPath -Raw -ErrorAction SilentlyContinue
        $stderr = Get-Content -LiteralPath $stderrPath -Raw -ErrorAction SilentlyContinue
        throw "Windows launcher scenario '$Name' failed with exit code $($process.ExitCode)`n$stdout`n$stderr"
    }
    return (Get-Content -LiteralPath $stdoutPath -Raw)
}

$quitOutput = Invoke-FoundationLauncher -InputLines @("q") -Name "quit"
if ($quitOutput -notmatch "Foundation Examples") {
    throw "Windows launcher did not print its heading"
}

$runOutput = Invoke-FoundationLauncher -InputLines @("1", "n") -Name "run-first"
if ($runOutput -notmatch "Process finished with exit code 0") {
    throw "Windows launcher did not report a successful example"
}

Write-Host "Validated $expectedCount Debug, Release, and exported examples for $Preset"
