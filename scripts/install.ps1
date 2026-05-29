param([Parameter(Mandatory=$true)][string]$Preset,[string]$Prefix="dist/$Preset")
cmake --install "build/$Preset" --prefix $Prefix
exit $LASTEXITCODE
