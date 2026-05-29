param(
    [Parameter(Mandatory=$true)]
    [string]$Preset,

    [string]$Config = "Release",

    [string]$Prefix = "dist/$Preset"
)

cmake --preset $Preset

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

cmake --build --preset $Preset --config $Config

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

cmake --install "build/$Preset" --config $Config --prefix $Prefix

exit $LASTEXITCODE