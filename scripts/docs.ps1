param(
    [string]$Preset = "documentation"
)

cmake --preset $Preset

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

cmake --build --preset $Preset --target docs

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

$IndexA = "build/$Preset/docs/html/index.html"
$IndexB = "build/$Preset/html/index.html"

if (Test-Path $IndexA) {
    Start-Process $IndexA
}
elseif (Test-Path $IndexB) {
    Start-Process $IndexB
}
else {
    Write-Host "Documentation was built, but index.html was not found in the expected paths."
}
