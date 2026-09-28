$script:FoundationRoot = Split-Path -Parent $PSScriptRoot
$script:FoundationBuildRoot = Join-Path $script:FoundationRoot "build"
$script:FoundationDistRoot = Join-Path $script:FoundationRoot "dist"
$script:FoundationRoModularScripts = Join-Path `
    $script:FoundationRoot `
    "tools/RoModularBuild/scripts"

$roModularCommon = Join-Path $script:FoundationRoModularScripts "common.ps1"
if (-not (Test-Path -LiteralPath $roModularCommon -PathType Leaf)) {
    throw "RoModularBuild is unavailable; initialize tools/RoModularBuild with git submodule update --init --recursive"
}

$env:ROMODULAR_PROJECT_ROOT = $script:FoundationRoot
$env:ROMODULAR_BUILD_ROOT = $script:FoundationBuildRoot
$env:ROMODULAR_DIST_ROOT = $script:FoundationDistRoot
$env:ROMODULAR_PROJECT_LABEL = "Foundation"
$env:ROMODULAR_CONFIGURE_COMMAND = "scripts/configure.ps1"
$env:ROMODULAR_DEFAULT_CONFIGURATION = "Debug"
$env:ROMODULAR_DOCUMENTATION_PRESET = "documentation"
$env:ROMODULAR_DOCUMENTATION_CONFIGURATION = "Release"
$env:ROMODULAR_INSTALL_CONFIGURATION = "Release"
$env:ROMODULAR_TESTING_CACHE_ARGUMENT = "-DFOUNDATION_TESTING=ON"
$env:ROMODULAR_EXAMPLES_CACHE_ARGUMENT = "-DFOUNDATION_EXAMPLES=ON"
