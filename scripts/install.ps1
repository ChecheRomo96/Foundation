param(
    [string]$Preset = "windows_msvc_x64_debug",
    [string]$Configuration = ""
)

cmake --preset $Preset

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

cmake --build --preset $Preset

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}

$BuildDir = "build/$Preset"

if ($Preset -match "_debug$") {
    $Configuration = "Debug"
}
elseif ($Preset -match "_release$" -or $Preset -match "_docs$") {
    $Configuration = "Release"
}

if ($Configuration -ne "") {
    cmake --install $BuildDir --config $Configuration
}
else {
    cmake --install $BuildDir
}

if ($LASTEXITCODE -ne 0) {
    exit $LASTEXITCODE
}