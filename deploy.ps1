# deploy.ps1 — Build, create layout, and register the packaged app
# Run from the shell-tasks-test project root directory.
# Requires elevated (Administrator) PowerShell for Add-AppxPackage.

param(
    [string]$Configuration = "Debug",
    [string]$Platform = "x64"
)

$ErrorActionPreference = "Stop"
$ProjectRoot = $PSScriptRoot

# 1. Build
Write-Host "Building $Configuration|$Platform..." -ForegroundColor Cyan
$msbuild = "C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\MSBuild.exe"
& $msbuild "$ProjectRoot\shell-tasks-test.sln" /p:Configuration=$Configuration /p:Platform=$Platform /nologo /v:minimal
if ($LASTEXITCODE -ne 0) { throw "Build failed." }

# 2. Create layout directory
$LayoutDir = "$ProjectRoot\AppxLayout"
if (Test-Path $LayoutDir) { Remove-Item $LayoutDir -Recurse -Force }
New-Item -ItemType Directory -Path $LayoutDir -Force | Out-Null
New-Item -ItemType Directory -Path "$LayoutDir\Assets" -Force | Out-Null
New-Item -ItemType Directory -Path "$LayoutDir\Public" -Force | Out-Null

# 3. Copy files to layout
$BuildOutput = "$ProjectRoot\$Platform\$Configuration"
Copy-Item "$BuildOutput\shell-tasks-test.exe" "$LayoutDir\" -Force
Copy-Item "$ProjectRoot\Package.appxmanifest" "$LayoutDir\AppxManifest.xml" -Force
Copy-Item "$ProjectRoot\Assets\icon.png" "$LayoutDir\Assets\icon.png" -Force

# Also copy any required DLLs from the build output
Get-ChildItem "$BuildOutput\*.dll" -ErrorAction SilentlyContinue | ForEach-Object {
    Copy-Item $_.FullName "$LayoutDir\" -Force
}

Write-Host "Layout created at: $LayoutDir" -ForegroundColor Green

# 4. Unregister previous version (if any)
Write-Host "Removing previous registration (if any)..." -ForegroundColor Yellow
Get-AppxPackage -Name "ShellTasksTest" -ErrorAction SilentlyContinue | Remove-AppxPackage -ErrorAction SilentlyContinue

# 5. Register the package
Write-Host "Registering package..." -ForegroundColor Cyan
Add-AppxPackage -Register "$LayoutDir\AppxManifest.xml"

Write-Host "" -ForegroundColor Green
Write-Host "Deployed! Run with:" -ForegroundColor Green
Write-Host "  .\run.ps1" -ForegroundColor White
Write-Host "  (or launch 'Shell Tasks Test' from Start menu)" -ForegroundColor White
