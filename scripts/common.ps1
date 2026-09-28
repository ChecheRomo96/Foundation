. "$PSScriptRoot/romodular-adapter.ps1"
. (Join-Path $script:FoundationRoModularScripts "common.ps1")

function Invoke-FoundationCMake {
    param([Parameter(Mandatory = $true)][string[]]$Arguments)

    Invoke-RoModularCMake -Arguments $Arguments
}

function Get-FoundationBuildDirectory {
    param([Parameter(Mandatory = $true)][string]$Preset)
    return Get-RoModularBuildDirectory -Preset $Preset
}

function Assert-FoundationPreset {
    param([Parameter(Mandatory = $true)][string]$Preset)

    Assert-RoModularPreset -Preset $Preset
}

function Get-FoundationConfiguration {
    param(
        [Parameter(Mandatory = $true)][string]$Preset,
        [string]$Configuration = "",
        [string]$DefaultConfiguration = "Debug"
    )

    return Get-RoModularConfiguration `
        -Preset $Preset `
        -Configuration $Configuration `
        -DefaultConfiguration $DefaultConfiguration
}

function Assert-FoundationConfiguration {
    param([Parameter(Mandatory = $true)][string]$Configuration)

    Assert-RoModularConfiguration -Configuration $Configuration
}

function Assert-FoundationConfigured {
    param([Parameter(Mandatory = $true)][string]$Preset)

    Assert-RoModularConfigured -Preset $Preset
}

function Resolve-FoundationPath {
    param([Parameter(Mandatory = $true)][string]$Path)

    return Resolve-RoModularPath -Path $Path
}

function Assert-FoundationDistChild {
    param([Parameter(Mandatory = $true)][string]$Path)

    Assert-RoModularDistChild -Path $Path
}
