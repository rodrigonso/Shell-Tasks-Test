# run.ps1 — Launch the registered packaged app
# The app must be deployed first with deploy.ps1

$ErrorActionPreference = "Stop"

$pkg = Get-AppxPackage -Name "ShellTasksTest" -ErrorAction SilentlyContinue
if (-not $pkg) {
    Write-Host "App not registered. Run deploy.ps1 first." -ForegroundColor Red
    exit 1
}

$appId = ($pkg.PackageFamilyName + "!App")
Write-Host "Launching: $appId" -ForegroundColor Cyan
Start-Process "shell:AppsFolder\$appId"
