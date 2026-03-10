[CmdletBinding()]
param()

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
Push-Location $repoRoot
try {
    Write-Host "[CHECK] Suche nach typischen privaten Dateien ..."
    $patterns = @(
        "Secrets.h",
        ".env",
        "secrets_local.h"
    )

    foreach ($pattern in $patterns) {
        $hits = Get-ChildItem -Recurse -Force -File | Where-Object { $_.Name -eq $pattern }
        foreach ($hit in $hits) {
            Write-Warning "Gefunden: $($hit.FullName)"
        }
    }

    Write-Host "[CHECK] Suche nach Build-/IDE-Resten ..."
    $dirs = @(".pio", ".vscode", "__pycache__")
    foreach ($dir in $dirs) {
        $hits = Get-ChildItem -Recurse -Force -Directory | Where-Object { $_.Name -eq $dir }
        foreach ($hit in $hits) {
            Write-Warning "Gefunden: $($hit.FullName)"
        }
    }

    Write-Host "[CHECK] Fertig. Warnungen vor Commit manuell pruefen."
}
finally {
    Pop-Location
}
