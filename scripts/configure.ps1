param(
    [Parameter(Mandatory = $true, Position = 0)]
    [string]$Preset,

    [switch]$Fresh,

    [Parameter(ValueFromRemainingArguments = $true)]
    [string[]]$CMakeArguments
)

. "$PSScriptRoot/common.ps1"

$arguments = @("--preset", $Preset)
if ($Fresh) {
    $arguments += "--fresh"
}
if ($CMakeArguments) {
    $arguments += $CMakeArguments
}

Invoke-FoundationCMake -Arguments $arguments

