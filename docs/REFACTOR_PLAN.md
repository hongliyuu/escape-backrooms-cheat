# 项目目录结构重构计划

> 目标:将目录结构规范化,使 `.sln` 位于仓库根目录,第三方库与 SDK 提升至顶层,源码归入 `src/`。
> 前置条件:**先解决 SDK 版本问题**(当前 SDK 针对游戏 1.20(2841),游戏更新后偏移失效导致崩溃),再执行本重构。

---

## 一、现状分析

### 当前结构
```
escape-backrooms-cheat/              ← git 仓库根
├── .gitignore  .gitattributes  LICENSE  README.md
├── .vscode/settings.json
└── cheat_etb/                       ← VS 解决方案目录(多一层)
    ├── cheat_etb.sln                ← 解决方案文件(应在根)
    ├── cheat/                       ← cheat.dll 项目
    │   ├── *.cpp *.h *.rc
    │   ├── minhook/                 ← 第三方库(埋在项目内)
    │   └── sdk/SDK/                 ← UE4 SDK(埋在项目内)
    └── loader/                      ← 启动器.exe 项目
        ├── *.cpp *.h *.rc
        └── *.vcxproj*
```

### 现存问题

| 问题 | 说明 | 严重度 |
|------|------|--------|
| `.sln` 不在仓库根 | 需进入 `cheat_etb/` 才能打开,不便 | 中 |
| `cheat_etb/` 多余层级 | 仓库根与解决方案根分离,结构冗余 | 中 |
| 第三方库埋在项目内 | `minhook/` 在 `cheat/` 内,不符合 `third_party/` 规范 | 低 |
| SDK 埋在项目内 | `sdk/SDK/` 在 `cheat/` 内,不利于独立更新 | 中 |
| **sln 残留 injector 引用** | A 类清理遗漏,`injector\injector.vcxproj` 已删但 sln 仍引用 → VS 打开报错 | **高(需立即修)** |

### 当前路径依赖关系
- `cheat_etb.sln` → `cheat\cheat.vcxproj`、`loader\loader.vcxproj`(相对 sln 目录)
- `cheat.vcxproj` → `minhook\src\*.c/h`、`sdk\SDK\*.cpp`(相对 cheat 项目目录)
- `loader.rc` → `..\cheat\x64\Release\cheat.dll`(相对 loader 项目目录,引用 cheat 输出)
- 输出目录:默认 `$(Platform)\$(Configuration)\`(相对各项目目录)

---

## 二、目标结构

```
escape-backrooms-cheat/              ← 仓库根 = 解决方案根
├── .gitignore  .gitattributes  LICENSE  README.md
├── .vscode/settings.json
├── cheat_etb.sln                    ← 移到根目录
├── src/                             ← 源码目录
│   ├── cheat/                       ← cheat.dll(仅源码 + vcxproj)
│   │   ├── *.cpp *.h *.rc
│   │   └── cheat.vcxproj*
│   └── loader/                      ← 启动器.exe
│       ├── *.cpp *.h *.rc
│       └── loader.vcxproj*
└── third_party/                     ← 第三方依赖
    ├── minhook/                     ← 从 cheat/minhook/ 提升
    └── ue4-sdk/                     ← 从 cheat/sdk/SDK/ 提升
```

### 设计原则
- **仓库根 = 解决方案根**:`.sln` 在根,直接打开
- **源码与依赖分离**:`src/` 放自有代码,`third_party/` 放外部依赖
- **SDK 独立**:`third_party/ue4-sdk/` 便于整体替换更新
- **loader↔cheat 相对关系不变**:都在 `src/` 下平级,`loader.rc` 的 `..\cheat\x64\Release\cheat.dll` 路径无需改动

---

## 三、详细改动清单

### 阶段 0:前置修复(立即执行,独立于重构)
**修复 sln 残留 injector 引用**——VS 打开 sln 当前会报错。

文件:`cheat_etb/cheat_etb.sln`
- 删除第 8-9 行(injector Project 节点)
- 删除第 28-35 行(injector 的 ProjectConfiguration 配置,GUID `{08A949C4-...}`)

> 此修复与重构无关,建议立即单独提交。

### 阶段 1:移动文件(物理迁移)

| 操作 | 源 | 目标 |
|------|----|----|
| 移动 sln | `cheat_etb/cheat_etb.sln` | `cheat_etb.sln`(根) |
| 移动 cheat | `cheat_etb/cheat/` | `src/cheat/` |
| 移动 loader | `cheat_etb/loader/` | `src/loader/` |
| 移动 minhook | `src/cheat/minhook/` | `third_party/minhook/` |
| 移动 SDK | `src/cheat/sdk/SDK/` | `third_party/ue4-sdk/` |
| 删除空 `cheat_etb/` | (移动后空目录) | — |

> 用 `git mv` 保留历史。SDK 文件量大(数百 .hpp/.cpp),`git mv` 整目录。

### 阶段 2:修改 `cheat_etb.sln`(已移到根)

**2.1 更新项目路径**(相对 sln 新位置):
```
原:Project(...) = "cheat", "cheat\cheat.vcxproj", "{...}"
新:Project(...) = "cheat", "src\cheat\cheat.vcxproj", "{...}"

原:Project(...) = "loader", "loader\loader.vcxproj", "{...}"
新:Project(...) = "loader", "src\loader\loader.vcxproj", "{...}"
```

**2.2 移除 injector 引用**(同阶段 0,若未单独做):
- 删除 injector Project 节点 + 其 ProjectConfiguration 配置块

### 阶段 3:修改 `src/cheat/cheat.vcxproj`

**3.1 更新 minhook 引用路径**(相对 `src/cheat/` → `third_party/minhook/`):
```
原:minhook\src\buffer.h
新:..\..\third_party\minhook\src\buffer.h

原:minhook\src\hde\hde32.c
新:..\..\third_party\minhook\src\hde\hde32.c
(所有 minhook\ 开头的 ClInclude/ClCompile 同理)
```
涉及约 12 个 ClInclude + 5 个 ClCompile(见 vcxproj 第 162-197 行)。

**3.2 更新 SDK 引用路径**(相对 `src/cheat/` → `third_party/ue4-sdk/`):
```
原:sdk\SDK\AdvancedSessions_functions.cpp
新:..\..\third_party\ue4-sdk\AdvancedSessions_functions.cpp
(所有 sdk\SDK\ 开头的 ClCompile 同理)
```
涉及数百个 .cpp/.hpp(见 vcxproj 第 202 行起)。**建议用脚本批量替换**,避免遗漏。

**3.3 检查 AdditionalIncludeDirectories**(当前无,确认无需补):
- 若代码中 `#include "SDK/..."` 等,可能需补 `..\..\third_party\ue4-sdk;..\..\third_party\minhook\include` 到 AdditionalIncludeDirectories
- 当前 grep 未发现该配置,需编译验证后决定

### 阶段 4:检查 `src/loader/loader.vcxproj` 与 `loader.rc`

**4.1 loader.vcxproj**:无第三方依赖,路径引用无需改(项目内文件相对路径不变)。

**4.2 loader.rc**(已改过一次):
```
当前:IDR_DLL1 DLL "..\\cheat\\x64\\Release\\cheat.dll"
重构后:仍为 "..\\cheat\\x64\\Release\\cheat.dll"  ← 无需改动
```
原因:loader 与 cheat 都在 `src/` 下平级,相对关系不变。✓

### 阶段 5:输出目录(可选优化)

当前各项目输出在自己目录下(`src/cheat/x64/Release/cheat.dll`)。**保持现状即可**,因 `loader.rc` 依赖 `..\cheat\x64\Release\cheat.dll` 相对路径。

如需统一输出到 `build/`,需同步改 `loader.rc`,风险增加。**本次重构不建议改输出目录**。

---

## 四、执行步骤(顺序)

```
1. [阶段0] 修 sln injector 残留 → 编译验证 → 提交
2. [前置] 确认 SDK 已更新可用,游戏不崩溃
3. [阶段1] git mv 移动文件(sln + src + third_party)
4. [阶段2] 改 sln 项目路径 + 移除 injector 引用
5. [阶段3] 改 cheat.vcxproj 的 minhook/sdk 路径(脚本批量替换)
6. [阶段4] 确认 loader.rc 无需改
7. 编译验证:cheat.dll + 启动器.exe(x64 Release + Win32)
8. 运行测试:启动器注入游戏,确认不崩溃(依赖阶段2的 SDK)
9. 提交重构
```

---

## 五、风险与验证

### 风险点
| 风险 | 应对 |
|------|------|
| vcxproj 路径替换遗漏 | 用脚本批量替换 + grep 验证无残留旧路径 |
| SDK 文件多,git mv 慢/出错 | 整目录 `git mv`,完成后 `git status` 核对 |
| `#include` 路径在源码内(非 vcxproj) | grep 源码 `#include.*minhook|#include.*SDK`,确认是否需改 |
| 编译失败(路径错) | 每阶段后编译验证,逐步定位 |
| loader.rc 路径失效 | 确认 cheat/loader 平级关系未变(已在阶段4分析) |

### 验证清单
- [ ] `git status` 显示文件移动(rename)而非删除+新增
- [ ] `cheat_etb.sln` 在根目录,VS 打开无报错
- [ ] sln 无 injector 引用
- [ ] grep vcxproj 无 `minhook\`、`sdk\SDK\` 旧路径残留
- [ ] 编译 cheat.dll 成功(x64 Release)
- [ ] 编译 启动器.exe 成功(x64 Release)
- [ ] 启动器.exe 运行,游戏注入不崩溃(需 SDK 已更新)

---

## 六、不在本次重构范围

- **SDK 版本更新**:独立任务,需用 UE4 Dumper 针对当前游戏版本重新生成
- **输出目录统一到 build/**:涉及 loader.rc 联动,风险高,本次不做
- **代码逻辑重构**(如 menu.cpp 菜单结构):与目录结构无关,不涉及
- **第三方库改用 vcpkg/NuGet**:minhook 体量小,内嵌可接受,不引入包管理器
