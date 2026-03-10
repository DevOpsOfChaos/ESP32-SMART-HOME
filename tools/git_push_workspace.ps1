[CmdletBinding()]
param(
    [string]$Branch = "main",
    [string]$Message
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    throw "git wurde nicht gefunden. Bitte Git installieren."
}

function Invoke-Git {
    param([Parameter(Mandatory = $true)][string[]]$Args)
    & git @Args
    if ($LASTEXITCODE -ne 0) {
        throw "Git fehlgeschlagen: git $($Args -join ' ') (ExitCode=$LASTEXITCODE)"
    }
}

function Get-GitOutput {
    param([Parameter(Mandatory = $true)][string[]]$Args)
    $out = & git @Args
    if ($LASTEXITCODE -ne 0) {
        throw "Git fehlgeschlagen: git $($Args -join ' ') (ExitCode=$LASTEXITCODE)"
    }
    return $out
}

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
Push-Location $repoRoot
try {
    if (-not (Test-Path -LiteralPath ".git")) {
        throw "Kein Git-Repository gefunden: $repoRoot"
    }

    $currentBranch = ((Get-GitOutput -Args @("rev-parse", "--abbrev-ref", "HEAD")) | Select-Object -First 1).Trim()
    if ($currentBranch -ne $Branch) {
        Write-Host "[SYNC] Wechsel auf Branch $Branch ..."
        Invoke-Git -Args @("checkout", $Branch)
    }

    $paths = @(
        "README.md",
        "CHANGELOG.md",
        ".gitignore",
        "docs",
        "firmware",
        "server",
        "hardware",
        "tools",
        "PROTOKOLL"
    )

    Write-Host "[SYNC] Stage: $($paths -join ', ')"
    $addArgs = @("add", "-A", "--") + $paths
    Invoke-Git -Args $addArgs

    $staged = Get-GitOutput -Args @("diff", "--cached", "--name-only")
    if (-not $staged) {
        Write-Host "[SYNC] Keine Aenderungen zum Commit vorhanden."
        return
    }

    if ([string]::IsNullOrWhiteSpace($Message)) {
        $timestamp = Get-Date -Format "yyyy-MM-dd HH:mm"
        $Message = "chore(sync): projektstand aktualisiert ($timestamp)"
    }

    Write-Host "[SYNC] Commit ..."
    Invoke-Git -Args @("commit", "-m", $Message)

    Write-Host "[SYNC] Push origin/$Branch ..."
    Invoke-Git -Args @("push", "origin", $Branch)

    Write-Host "[SYNC] Fertig."
    Invoke-Git -Args @("status", "--short", "--branch")
}
finally {
    Pop-Location
}
