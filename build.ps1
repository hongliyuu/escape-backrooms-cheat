param(
    [string]$Config = "Release",
    [string]$Platform = "x64"
)
$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $MyInvocation.MyCommand.Path

# Locate MSBuild via vswhere
$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
if (-not (Test-Path $vswhere)) {
    Write-Host "ERROR: vswhere not found. Install Visual Studio." -ForegroundColor Red
    exit 1
}
$msbuild = & $vswhere -latest -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe" | Select-Object -First 1
if (-not $msbuild) {
    Write-Host "ERROR: MSBuild not found. Install 'MSBuild' component in VS Installer." -ForegroundColor Red
    exit 1
}

$platDir = if ($Platform -eq "x64") { "x64" } else { "Win32" }

function Build-Project($name, $vcxproj) {
    Write-Host ""
    Write-Host "=== Building $name ($Config|$Platform) ===" -ForegroundColor Cyan
    & $msbuild "$vcxproj" /p:Configuration=$Config /p:Platform=$Platform /m /v:m
    if ($LASTEXITCODE -ne 0) {
        Write-Host "FAILED: $name" -ForegroundColor Red
        exit 1
    }
    Write-Host "OK: $name" -ForegroundColor Green
}

Build-Project "cheat" "$root\src\cheat\cheat.vcxproj"
Build-Project "loader" "$root\src\loader\loader.vcxproj"

Write-Host ""
Write-Host "=== Build succeeded ===" -ForegroundColor Green
$cheatOut = "$root\src\cheat\$platDir\$Config\cheat.dll"
Write-Host "cheat.dll: $cheatOut"
$exe = Get-ChildItem "$root\src\loader\$platDir\$Config\*.exe" -ErrorAction SilentlyContinue | Select-Object -First 1
if ($exe) { Write-Host "loader:    $($exe.FullName)" }
