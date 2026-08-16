param(
    [string]$Preset = "documentation"
)

cmake --preset $Preset

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

cmake --build "build/$Preset" --target docs

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

$SourceHtml = "build/$Preset/docs/html"
$DistDocs = "dist/$Preset"

if (Test-Path $DistDocs) {
    Remove-Item $DistDocs -Recurse -Force
}

New-Item -ItemType Directory -Path $DistDocs -Force | Out-Null

Move-Item "$SourceHtml/*" $DistDocs -Force

$Index = "$DistDocs/index.html"

if (Test-Path $Index) {
    Start-Process $Index
}
else {
    Write-Host "Documentation was built, but index.html was not found at:"
    Write-Host $Index
}