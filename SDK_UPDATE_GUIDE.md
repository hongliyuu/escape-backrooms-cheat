# SDK 更新指南

> 用 Dumper-7 重新生成 SDK,修复游戏版本更新导致的偏移失效(注入后崩溃)。
> 工具:https://github.com/Encryqed/Dumper-7(支持 UE4.11 - UE5.x)

## 偏移位置

[Basic.hpp:62-70](cheat_etb/cheat/sdk/SDK/Basic.hpp#L62-L70) 的 `Offsets` 命名空间,6 个 RVA(相对模块基址):

- `GObjects` / `GNames` / `GWorld` / `AppendString` / `ProcessEvent` / `ProcessEventIdx`

游戏更新后这些值失效 → SDK 访问错误地址 → UE4 崩溃。

## 步骤

### 1. 编译 Dumper-7

```powershell
git clone https://github.com/Encryqed/Dumper-7 D:\WorkSpace\Dumper-7
$msbuild = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.Component.MSBuild -find "MSBuild\**\Bin\MSBuild.exe" | Select-Object -First 1
& $msbuild "D:\WorkSpace\Dumper-7\Dumper\Dumper.sln" /p:Configuration=Release /p:Platform=x64 /m
```

产物:`D:\WorkSpace\Dumper-7\Dumper\x64\Release\Dumper-7.dll`

### 2. 注入游戏 dump

启动游戏进入主菜单,然后用任一方式注入 `Dumper-7.dll`:

**方式 A:System Informer**(图形界面)
右键游戏进程 → Miscellaneous → Inject DLL → 选 Dumper-7.dll

**方式 B:PowerShell 脚本**(需管理员)

```powershell
Add-Type -TypeDefinition @"
using System; using System.Runtime.InteropServices;
public class Injector {
    [DllImport("kernel32.dll")] public static extern IntPtr OpenProcess(uint a,bool b,uint c);
    [DllImport("kernel32.dll")] public static extern IntPtr VirtualAllocEx(IntPtr h,IntPtr a,uint s,uint t,uint p);
    [DllImport("kernel32.dll")] public static extern bool WriteProcessMemory(IntPtr h,IntPtr a,byte[] b,uint s,out uint w);
    [DllImport("kernel32.dll")] public static extern IntPtr GetProcAddress(IntPtr h,string n);
    [DllImport("kernel32.dll")] public static extern IntPtr GetModuleHandle(string n);
    [DllImport("kernel32.dll")] public static extern IntPtr CreateRemoteThread(IntPtr h,IntPtr a,uint s,IntPtr f,IntPtr p,uint fl,out uint t);
    [DllImport("kernel32.dll")] public static extern uint WaitForSingleObject(IntPtr h,uint t);
    [DllImport("kernel32.dll")] public static extern bool CloseHandle(IntPtr h);
    public static string Inject(uint pid,string dll){
        IntPtr h=OpenProcess(0x1F0FFF,false,pid);
        if(h==IntPtr.Zero) return "OpenProcess failed (need admin)";
        byte[] b=System.Text.Encoding.ASCII.GetBytes(dll+"\0");
        IntPtr m=VirtualAllocEx(h,IntPtr.Zero,(uint)b.Length,0x3000,0x40);
        uint w; WriteProcessMemory(h,m,b,(uint)b.Length,out w);
        IntPtr f=GetProcAddress(GetModuleHandle("kernel32.dll"),"LoadLibraryA");
        uint t; IntPtr th=CreateRemoteThread(h,IntPtr.Zero,0,f,m,0,out t);
        WaitForSingleObject(th,10000); CloseHandle(th); CloseHandle(h);
        return "OK";
    }
}
"@
$proc = Get-Process | Where-Object { $_.ProcessName -like "*scape*" -or $_.ProcessName -like "*Backrooms*" } | Select-Object -First 1
[Injector]::Inject($proc.Id, "Dumper-7.dll 的完整路径")
```

控制台显示 `Generating SDK took (Xms)` 即完成。SDK 输出到 `C:\Dumper-7\<FolderName>\CppSDK\`。

### 3. 替换 SDK

```powershell
Remove-Item cheat_etb\cheat\sdk -Recurse -Force
Copy-Item "C:\Dumper-7\<FolderName>\CppSDK" cheat_etb\cheat\sdk -Recurse
```

偏移由 Dumper-7 自动写入新 `Basic.hpp`,无需手动改。

### 4. 编译验证

```powershell
& $msbuild "cheat_etb\cheat\cheat.vcxproj" /p:Configuration=Release /p:Platform=x64 /m
& $msbuild "cheat_etb\loader\loader.vcxproj" /p:Configuration=Release /p:Platform=x64 /m
```

`cheat.vcxproj` 显式引用 8 个 `_functions.cpp`(AdvancedSessions/Backrooms/Basic/BPCharacter_Demo/BP_Scanner/CoreUObject/Engine/UMG),新 SDK 都有,**vcxproj 无需改动**。

### 5. 测试

运行 `cheat_etb\loader\x64\Release\启动器.exe`,确认游戏不崩溃、菜单正常。

## 注意

- **整体替换**:不能只改偏移,结构定义(`_classes.hpp`)也会随版本变化
- **版本一致**:dump 时的游戏版本 = 你要玩的版本
- **偏移是 RVA**:运行时 `GetModuleHandle(0) + 偏移` 得实际地址
- **备份用 git**:SDK 改动提交到 git,失败可 `git checkout` 回退
