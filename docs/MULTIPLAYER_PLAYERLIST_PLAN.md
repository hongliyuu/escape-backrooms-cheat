# 联机玩家列表显示功能实施方案

> 目标:让房间人数突破默认上限(4 人),在 Lobby、游戏内左上角、ESC 菜单三处显示所有连入玩家。
> 前置条件:SDK 已更新匹配当前游戏版本(见 [SDK_UPDATE_GUIDE.md](SDK_UPDATE_GUIDE.md))。
> 运行环境:cheat 仅在 host 端运行,其他客户端无 cheat。

---

## 一、问题现状

### 1.1 现象
- 三套玩家列表 UI **均上限 4 人**,第 5 人及以后不显示(不是"不显示",是"只能显示 4 个")
- `session::create_session()` 已实现但**未接入 menu**,功能不可用
- 人数选择条(故事模式/枢纽模式)上限为 4

### 1.2 根因
- `session.cpp` 改的是 **Steam/Epic 在线服务层**的 `NumPublicConnections`,**不等于** `ALobby_GS_C::MaxPlayers`
- 游戏 UI 看的是 `ALobby_GS_C` 的 `MaxPlayers`(槽位数)和 `PlayersConnected`(玩家数组)
- `UW_Lobby_PlayerList_C` 的 `RefreshPlayerList` 遍历 `PlayerListEntries` 数组(默认 4 个 Entry0~3),**不动态创建新 Entry**
- `UW_VOIP_PlayerList_C` 有 4 个预创建 Entry 字段,但 `VOIPPlayerList` 是 `UVerticalBox*` 容器,可 AddChild

### 1.3 关键限制:其他客户端无法显示超过 4 人

**结论:cheat 只在 host 端运行,其他客户端(无 cheat)无法显示超过 4 人。**

| 环节 | host 端 | 其他客户端(无 cheat) |
|------|---------|---------------------|
| `MaxPlayers` / `PlayersConnected` 数据同步 | ✅ host 改后自动同步 | ✅ 收到同步数据 |
| widget Entry 数量 | ✅ cheat 动态扩展 | ❌ 只有默认 4 个,无法扩展 |
| UI 显示 8 人 | ✅ 可实现 | ❌ **无法实现** |

**根本原因**:[`UW_Lobby_PlayerList_C::RefreshPlayerList`](../third_party/ue4-sdk/SDK/W_Lobby_PlayerList_parameters.hpp) 的参数结构只有 `Array_Get_Item`、`Array_Length`、`Less_IntInt`(遍历已有数组),**没有 `CreateWidget` / `Array_Add`**(不扩展数组)。其他客户端没有 cheat,无法扩展 `PlayerListEntries`,所以即使数据同步了也只能显示前 4 人。

> **设计取舍**:本方案只保证 **host 端** 三套 UI 显示 8 人。其他客户端保持 4 人(除非也装 cheat)。

---

## 二、三套 UI 对照

| 场景 | 容器 widget | 玩家列表 widget | 数据源 | 默认 Entry | 扩展方式 |
|------|-------------|----------------|--------|-----------|---------|
| **Lobby 准备阶段** | `UUI_Menu_Lobby_C.PlayerList` | `UW_Lobby_PlayerList_C` | `ALobby_GS_C::PlayersConnected` (Net+RepNotify) | Entry0~3 | Add 到 `PlayerListEntries` 数组 |
| **游戏内 ESC 菜单** | `UWB_SettingsPanel_Theme2_C.W_Lobby_PlayerList` | `UW_Lobby_PlayerList_C` | 同上 | 同上 | 同上 |
| **游戏内左上角** | (独立 widget) | `UW_VOIP_PlayerList_C` | `AGameStateBase::PlayerArray` (引擎同步) | Entry1~4 | AddChild 到 `VOIPPlayerList` (UVerticalBox) |

### 关键 SDK 引用
- [Lobby_GS_classes.hpp:23-40](../third_party/ue4-sdk/SDK/Lobby_GS_classes.hpp#L23-L40) — `ALobby_GS_C` 含 `PlayersConnected` / `MaxPlayers` (均带 `Net, RepNotify`)
- [W_Lobby_PlayerList_classes.hpp:21-53](../third_party/ue4-sdk/SDK/W_Lobby_PlayerList_classes.hpp#L21-L53) — `UW_Lobby_PlayerList_C` 字段与方法
- [W_VOIP_PlayerList_classes.hpp:21-31](../third_party/ue4-sdk/SDK/W_VOIP_PlayerList_classes.hpp#L21-L31) — `UW_VOIP_PlayerList_C` 字段
- [WB_SettingsPanel_Theme2_classes.hpp:57](../third_party/ue4-sdk/SDK/WB_SettingsPanel_Theme2_classes.hpp#L57) — ESC 菜单挂载点
- [UI_Menu_Lobby_classes.hpp:46](../third_party/ue4-sdk/SDK/UI_Menu_Lobby_classes.hpp#L46) — Lobby 菜单挂载点
- [Lobby_PlayerController_Nebula_classes.hpp:23-93](../third_party/ue4-sdk/SDK/Lobby_PlayerController_Nebula_classes.hpp#L23-L93) — `ALobby_PlayerController_Nebula_C` 含 `UI_MenuLobby` / `SettingsUI_Ref` / `OC_Update_ConnectedPlayersList`
- [UI_Menu_ModeSelection_classes.hpp:25-50](../third_party/ue4-sdk/SDK/UI_Menu_ModeSelection_classes.hpp#L25-L50) — 模式选择 + 人数滑块
- [W_CreateServer_classes.hpp:34-43](../third_party/ue4-sdk/SDK/W_CreateServer_classes.hpp#L34-L43) — 创建服务器 + 人数滑块
- [session.h](../src/cheat/session.h) / [session.cpp](../src/cheat/session.cpp) — 现有 create_session 实现

---

## 三、网络同步机制

UE4 是**服务器权威 + RepNotify**模型。只有带 `Net` 标记的字段才会网络同步。

| 操作 | host/client | 其他客户端可见 | 原因 |
|------|-------------|-------------|------|
| 本地调用 `RefreshPlayerList` | host 本地 | ❌ | widget 是本地 UI,不同步 |
| 本地改 `Slider_MaxPlayers->MaxValue` | host 本地 | ❌ | 本地 UI 操作 |
| **host 改 `Lobby_GS->MaxPlayers`** | host | ✅ 数据同步 | 带 `Net, RepNotify`,自动同步 |
| **host 调用 `Update_ConnectedPlayers_Array()`** | host | ✅ 数据同步 | 填充 `PlayersConnected`,自动同步 |
| host 调用 `session::create_session(N, 0, false)` | host | ✅ 间接影响 | 改 Online Session,影响允许多少人连入 |

**关键区分**:
- **数据层**:host 改 `MaxPlayers` / `PlayersConnected` → **所有客户端数据同步** ✅
- **UI 层**:`UW_Lobby_PlayerList_C` 的 Entry 数组是 widget 本地字段 → **不同步** ❌

所以其他客户端即使收到 8 人数据,widget 也只有 4 个 Entry,`RefreshPlayerList` 只能填前 4 个。

---

## 四、实施阶段(每阶段独立可验证)

> 原则:每阶段改动最小化,独立验证通过后再进入下一阶段。失败不影响已完成阶段。
> SDK 参数结构不能 100% 反映蓝图内部逻辑(UberGraph 调用不体现),关键结论需实测确认。

### 阶段 1:改人数选择条(独立,低风险)

**目标**:把模式选择/创建服务器界面的人数滑块上限从 4 改到 8。

**涉及 widget**:
- [`UUI_Menu_ModeSelection_C`](../third_party/ue4-sdk/SDK/UI_Menu_ModeSelection_classes.hpp#L25-L50) — 故事模式/枢纽模式选择 + `Slider_MaxPlayers`
- [`UW_CreateServer_C`](../third_party/ue4-sdk/SDK/W_CreateServer_classes.hpp#L34-L43) — 创建服务器 + `Slider_MaxPlayers`

**改法**:
```cpp
// 找到 widget 实例后
widget->Slider_MaxPlayers->MaxValue = 8.0f;
widget->Slider_MaxPlayers->SetValue(8.0f);
widget->UpdateMaxPlayers();  // 触发 UI 更新
```

**验证**:滑块能拖到 8,显示"8"。

**风险**:无,纯本地 UI 改动。

---

### 阶段 2:接入 menu UI + session 调用(依赖阶段 1)

**目标**:在 cheat 菜单加"联机房间"分类,提供人数输入框 + 创建按钮,启用 `session::create_session`。

**步骤**:
1. menu 加"联机房间"分类
2. 提供人数输入框(默认 8)
3. 点击"创建房间" → 调用 `session::create_session(N, 0, false)` + `session::open_lobby()`
4. 提供手动"刷新玩家列表"按钮(备用)

**验证**:host 点击创建房间后,游戏进入 Lobby,其他玩家能连入(最多 8 人)。

**风险**:
- `create_session` 调用失败 → 检查 OnlineSubsystem 是否初始化
- 人数超过游戏引擎支持 → 实测是否支持 8 人

---

### 阶段 3a:host 端数据同步(探测性,依赖阶段 2)

**目标**:host 端只改数据(MaxPlayers + PlayersConnected),**不扩展 widget Entry**,观察其他客户端是否自动显示超过 4 人。

> **此阶段是探测性的** — SDK 分析表明其他客户端无法自动显示(详见 1.3 节),但蓝图内部逻辑无法从 SDK 100% 确认。先只改数据,实测验证。

**步骤**:
1. **host 端改数据**:
   - 改 `Lobby_GS->MaxPlayers = N`
   - 调用 `Lobby_GS->Update_ConnectedPlayers_Array()` 填充 `PlayersConnected`
   - 手动触发 `OnRep_MaxPlayers()` / `OnRep_PlayersConnected()` 刷新

2. **不扩展 widget**:`UW_Lobby_PlayerList_C::PlayerListEntries` 保持默认 4 个 Entry

3. **观察结果**:

| 观察对象 | 自动显示 8 人? | 结论 | 后续 |
|----------|---------------|------|------|
| host 端 Lobby/ESC 列表 | 大概率 ❌(Entry 只有 4 个) | 需扩展 widget | 进入阶段 3b |
| 其他客户端 Lobby/ESC 列表 | 大概率 ❌(同上) | 确认限制 | 进入阶段 3b(仅 host) |
| host 端 Lobby/ESC 列表 | ✅(意外) | 游戏有动态创建机制 | 阶段 3b 跳过 |
| 其他客户端 Lobby/ESC 列表 | ✅(意外) | 游戏有动态创建机制 | 阶段 3b 跳过 |

**验证**:
- host 端和至少 1 个其他客户端同时观察 Lobby 列表
- 逐一让玩家加入(第 5、6、7、8 人),观察列表是否实时更新
- 记录:host 端显示几人?其他客户端显示几人?

**风险**:无,只改数据不扩展 widget,最坏情况是 UI 没变化。

---

### 阶段 3b:host 端 widget Entry 扩展(依赖阶段 3a 结果)

**前置条件**:阶段 3a 实测确认"host 端和/或其他客户端无法自动显示超过 4 人"。

**目标**:在需要扩展的端(host 端,或其他客户端如果也要支持)扩展 `UW_Lobby_PlayerList_C` 的 Entry 数组。

**步骤**:
1. **找到 `UW_Lobby_PlayerList_C` 实例**:
   - Lobby 列表:`ALobby_PlayerController_Nebula_C->UI_MenuLobby->PlayerList`
   - ESC 列表:`ALobby_PlayerController_Nebula_C->SettingsUI_Ref->W_Lobby_PlayerList`

2. **扩展 `PlayerListEntries` 数组**:
   - 检查 `PlayerListEntries.Num()` < N 时
   - 用 `UWidgetBlueprintLibrary::CreateWidget` 创建额外的 `UW_Lobby_PlayerList_Entry_C`
   - Add 到 `PlayerListEntries` 数组

3. **刷新数据**:
   - 构造 `TArray<FBPUniqueNetId>`(从 `PlayersConnected` 提取每个玩家的 UniqueNetId)
   - 调用 `RefreshPlayerList(NetIDs, MaxPlayers)` 填充所有 Entry

4. **自动刷新机制**:
   - 方案 A:定时器(每 0.5 秒检查 `PlayersConnected.Num()` 变化,变化时刷新)
   - 方案 B:hook `OnRep_PlayersConnected`,触发时自动刷新

**验证**:
- host 端 Lobby 列表显示 8 人(逐个加入验证)
- host 端 ESC 菜单列表显示 8 人(按 ESC 打开设置面板验证)
- 新玩家连入时,host 端列表自动更新

**风险**:
- `CreateWidget` 需在游戏线程执行,传入正确的 Outer 和 Class
- Entry 扩容后索引错位 → 扩容后立即调用 `RefreshPlayerList`
- `PlayerListEntries` 是 `TArray`,Add 后需确认内存有效
- 其他客户端(无 cheat)仍只显示 4 人(阶段 3a 已确认,非 bug)

---

### 阶段 4:host 端 VOIP 左上角扩展(独立,中等难度)

**目标**:让 `UW_VOIP_PlayerList_C` 支持显示超过 4 人。

> **前置探测**:同阶段 3a,先只改数据(host 改 MaxPlayers + 触发刷新),观察 host 端左上角 VOIP 列表是否自动显示超过 4 人。如果自动显示,跳过本阶段。

**步骤**(如果探测确认需要扩展):
1. 找到 `UW_VOIP_PlayerList_C` 实例(从 `ALobby_PlayerController_Nebula_C->VOIP_HUD_Ref` 遍历找)
2. 用 `CreateWidget` 创建额外的 `UW_VOIP_PlayerList_Entry_C`
3. 调用 `VOIPPlayerList->AddChild(entry)` 加到 `UVerticalBox` 容器
4. hook `RefreshPlayerListings`,在原逻辑执行后手动填充第 5+ 个 Entry 的数据

**说明**:
- `VOIPPlayerList` 是 `UVerticalBox*` 容器(不是固定数组),可 AddChild
- `PlayerStates` 来自 `AGameStateBase::PlayerArray`(引擎同步),host 端数据完整

**验证**:host 端游戏内左上角列表显示 8 人。

**风险**:
- hook `RefreshPlayerListings` 可能与游戏自身逻辑冲突
- Entry 样式可能不一致(需确认 CreateWidget 的 Class 正确)

---

## 五、执行顺序与验证清单

```
阶段 1  → 验证滑块可拖到 8
  ↓
阶段 2  → 验证能创建 8 人房间
  ↓
阶段 3a → 探测:host 改数据不扩展 widget,观察谁自动显示
  ↓
  ├─ 自动显示 → 阶段 3b 跳过,直接进阶段 4
  └─ 不显示   → 阶段 3b 扩展 host 端 widget
                ↓
阶段 4  → (同样先探测)host 端左上角 VOIP 列表
```

### 每阶段验证清单
- [ ] **阶段 1**:模式选择滑块可拖到 8
- [ ] **阶段 2**:host 创建 8 人房间,其他玩家能连入
- [ ] **阶段 3a**:记录 host 端和其他客户端的 Lobby/ESC 列表显示人数
- [ ] **阶段 3b**(如需要):host 端 Lobby 列表显示 8 人;host 端 ESC 菜单列表显示 8 人
- [ ] **阶段 4**(如需要):host 端游戏内左上角列表显示 8 人

### 已知限制(非 bug,阶段 3a 实测确认后更新)
- 其他客户端(无 cheat)的玩家列表可能仍上限 4 人
- 8 人游戏是否触发反作弊需实测

---

## 六、不在本次范围

- **其他客户端 UI 扩展**:需每个客户端装 cheat,不在本次方案
- **游戏逻辑平衡**:8 人游戏的难度/出生点/任务触发不在 cheat 职责内
- **网络性能优化**:8 人同步可能增加网络负担,需游戏自己处理
