<#
.SYNOPSIS
    编译脚本：使用 MSBuild 构建 cheat.dll 与 启动器.exe

.DESCRIPTION
    依赖：Visual Studio 2022（含 MSBuild 组件）+ Windows 10 SDK + MSVC v143 工具集

.NOTES
    首次运行若报 "在此系统上禁止运行脚本"（PowerShell 执行策略），
    任选其一执行一次：

    方案 A（仅本次绕过，不修改系统设置）：
        powershell -ExecutionPolicy Bypass -File .\build.ps1 [-Clean] [-Config <C>] [-Platform <P>]

    方案 B（永久允许本地脚本，推荐；仅执行一次，之后直接 .\build.ps1 即可）：
        Set-ExecutionPolicy -Scope CurrentUser RemoteSigned -Force

    说明：-Force 跳过 Y/N 确认提示；RemoteSigned 允许本地脚本运行，
          网络下载的脚本仍需签名，安全性合理。

.EXAMPLE
    # 默认：x64 Release 增量编译（需先按方案 B 解除执行策略限制）
    .\build.ps1
.EXAMPLE
    # 指定配置与平台
    .\build.ps1 -Config Debug -Platform x86
.EXAMPLE
    # 全量重编译（先清理）
    .\build.ps1 -Clean
.EXAMPLE
    # 未配置执行策略时，一次性绕过并全量重编译
    powershell -ExecutionPolicy Bypass -File .\build.ps1 -Clean
#>
param(
    [string]$Config = "Release",
    [string]$Platform = "x64",
    [switch]$Clean
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
    $action = if ($Clean) { "Rebuild" } else { "Build" }
    Write-Host ""
    Write-Host "=== $action $name ($Config|$Platform) ===" -ForegroundColor Cyan
    & $msbuild "$vcxproj" /t:$action /p:Configuration=$Config /p:Platform=$Platform /m /v:m
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
