param([Parameter(Mandatory=$true)][string]$Preset)
cmake --preset $Preset
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
cmake --build --preset $Preset
exit $LASTEXITCODE
