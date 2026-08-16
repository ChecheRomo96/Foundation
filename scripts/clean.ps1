param([string]$Preset="")
if ($Preset -eq "") {
    Remove-Item -Recurse -Force build -ErrorAction SilentlyContinue
    Remove-Item -Recurse -Force dist -ErrorAction SilentlyContinue
} else {
    Remove-Item -Recurse -Force "build/$Preset" -ErrorAction SilentlyContinue
    Remove-Item -Recurse -Force "dist/$Preset" -ErrorAction SilentlyContinue
}
