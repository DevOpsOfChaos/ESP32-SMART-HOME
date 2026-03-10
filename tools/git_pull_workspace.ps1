[CmdletBinding()]
param(
    [string]$Branch = "main"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    throw "git wurde nicht gefunden. Bitte Git installieren."
}

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
Push-Location $repoRoot
try {
    if (-not (Test-Path -LiteralPath ".git")) {
        throw "Kein Git-Repository gefunden: $repoRoot"
    }

    Write-Host "[SYNC] Hole aktuellen Stand von origin/$Branch ..."
    git fetch --prune origin

    Write-Host "[SYNC] Wechsel auf Branch $Branch ..."
    git checkout $Branch

    Write-Host "[SYNC] Setze lokalen Stand hart auf origin/$Branch ..."
    git reset --hard "origin/$Branch"

    Write-Host "[SYNC] Entferne untracked/ignored Dateien ..."
    git clean -ffdx

    Write-Host "[SYNC] Fertig."
    git status --short --branch
}
finally {
    Pop-Location
}
