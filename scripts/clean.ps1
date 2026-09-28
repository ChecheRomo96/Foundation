param(
    [Parameter(Position = 0)]
    [string]$Preset = "",

    [switch]$All,
    [switch]$Dist
)

. "$PSScriptRoot/romodular-adapter.ps1"

$parameters = @{
    Preset = $Preset
    All = $All
    Dist = $Dist
}

& (Join-Path $script:FoundationRoModularScripts "clean.ps1") @parameters
