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
$effectiveCMakeArguments = @(
    $CMakeArguments | Where-Object {
        -not [string]::IsNullOrWhiteSpace($_)
    }
)
if ($effectiveCMakeArguments.Count -gt 0) {
    $arguments += $effectiveCMakeArguments
}

Invoke-FoundationCMake -Arguments $arguments

