param(
    [string]$Preset = "windows_msvc_x64_debug"
)

cmake --preset $Preset

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

cmake --build --preset $Preset

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}