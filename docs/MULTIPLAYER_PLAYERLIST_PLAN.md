# 联机玩家列表显示功能实施方案

> 目标:修改游戏原生 UI,让房间人数突破默认上限(4 人),在 Lobby、游戏内左上角、ESC 菜单三处实时显示所有连入玩家。
> 前置条件:SDK 已更新匹配当前游戏版本(见 [SDK_UPDATE_GUIDE.md](SDK_UPDATE_GUIDE.md))。

---

## 一、问题现状

### 1.1 现象
- cheat 的 `session::create_session()` 已实现(改 Online Session 的 `NumPublicConnections`),但**未接入 menu**,功能不可用
- 游戏内左上角、ESC 菜单的玩家列表**默认上限 4 人**,第 5 人及以后不显示
- Lobby 准备阶段新玩家连入时,`PlayersConnected` 未被主动刷新,列表滞后

### 1.2 根因
- `session.cpp` 改的是 **Steam/Epic 在线服务层**的 `NumPublicConnections`,**不等于** `ALobby_GS_C::MaxPlayers`
- 游戏 UI 看的是 `ALobby_GS_C` 的 `MaxPlayers`(槽位数)和 `PlayersConnected`(玩家数组),两者独立
- `UW_Lobby_PlayerList_C` 有 4 个固定 Entry + 1 个动态数组,但 `RefreshPlayerList` 不动态创建新 Entry
- `UW_VOIP_PlayerList_C` 只有 4 个预创建 Entry 字段,但 `VOIPPlayerList` 是 `UVerticalBox*` 容器,可 AddChild

---

## 二、三套 UI 对照

| 场景 | 容器 widget | 玩家列表 widget | 数据源 | 固定槽位 | 可扩展 |
|------|-------------|----------------|--------|---------|--------|
| **Lobby 准备阶段** | `UUI_Menu_Lobby_C.PlayerList` | `UW_Lobby_PlayerList_C` | `ALobby_GS_C::PlayersConnected` (Net+RepNotify) | Entry0~3 (4 个) | ✅ PlayerListEntries 动态数组 |
| **游戏内 ESC 菜单** | `UWB_SettingsPanel_Theme2_C.W_Lobby_PlayerList` | `UW_Lobby_PlayerList_C` | 同上 | 同上 | ✅ 同上 |
| **游戏内左上角** | (独立 widget) | `UW_VOIP_PlayerList_C` | `AGameStateBase::PlayerArray` (引擎同步) | W_VOIP_PlayerList_Entry1~4 | ✅ VOIPPlayerList 是 UVerticalBox 容器 |

### 关键 SDK 引用
- [Lobby_GS_classes.hpp:23-40](../third_party/ue4-sdk/SDK/Lobby_GS_classes.hpp#L23-L40) — `ALobby_GS_C` 含 `PlayersConnected` / `MaxPlayers` (均带 `Net, RepNotify`)
- [W_Lobby_PlayerList_classes.hpp:21-53](../third_party/ue4-sdk/SDK/W_Lobby_PlayerList_classes.hpp#L21-L53) — `UW_Lobby_PlayerList_C` 字段与方法
- [W_VOIP_PlayerList_classes.hpp:21-31](../third_party/ue4-sdk/SDK/W_VOIP_PlayerList_classes.hpp#L21-L31) — `UW_VOIP_PlayerList_C` 字段
- [WB_SettingsPanel_Theme2_classes.hpp:57](../third_party/ue4-sdk/SDK/WB_SettingsPanel_Theme2_classes.hpp#L57) — ESC 菜单挂载点
- [UI_Menu_Lobby_classes.hpp:46](../third_party/ue4-sdk/SDK/UI_Menu_Lobby_classes.hpp#L46) — Lobby 菜单挂载点
- [UI_Menu_ModeSelection_classes.hpp:25-50](../third_party/ue4-sdk/SDK/UI_Menu_ModeSelection_classes.hpp#L25-L50) — 模式选择 + 人数滑块
- [W_CreateServer_classes.hpp:34-43](../third_party/ue4-sdk/SDK/W_CreateServer_classes.hpp#L34-L43) — 创建服务器 + 人数滑块
- [session.h](../src/cheat/session.h) / [session.cpp](../src/cheat/session.cpp) — 现有 create_session 实现

---

## 三、网络同步机制(关键)

UE4 是**服务器权威 + RepNotify**模型。只有带 `Net` 标记的字段才会网络同步。

| 操作 | host/client | 其他玩家可见 | 原因 |
|------|-------------|-------------|------|
| 本地调用 `RefreshPlayerList` | 任意端 | ❌ 看不到 | widget 是本地 UI,不同步 |
| 本地改 `PlayerStates` 数组 | 任意端 | ❌ 看不到 | widget 字段,不同步 |
| 本地改 `Slider_MaxPlayers->MaxValue` | host 本地 | ❌ 看不到 | 本地 UI 操作 |
| **host 改 `Lobby_GS->MaxPlayers`** | host | ✅ 所有客户端 | 带 `Net, RepNotify`,自动同步+触发 `OnRep_MaxPlayers` |
| **host 调用 `Update_ConnectedPlayers_Array()`** | host | ✅ 所有客户端 | 填充 `PlayersConnected` (带 `Net, RepNotify`),自动同步 |
| host 调用 `session::create_session(N, 0, false)` | host | ✅ 间接影响 | 改 Online Session 的 `NumPublicConnections`,影响允许多少人连入 |

### 核心结论
要让"所有玩家都能在游戏 UI 上看到正确人数和玩家列表",**必须在 host 端操作**:
1. **host** 改 `Lobby_GS->MaxPlayers = N` → 自动同步 → 所有客户端 `OnRep_MaxPlayers` → UI 槽位数更新
2. **host** 调用 `Update_ConnectedPlayers_Array()` → 服务器填充 `PlayersConnected` → 自动同步 → 所有客户端 `OnRep_PlayersConnected` → UI 玩家列表更新

**客户端单独改只能让自己看到,其他人看不到。**

**左上角 VOIP 列表例外**:`PlayerStates` 来源是 `AGameStateBase::PlayerArray`(引擎自动同步),所有客户端的 PlayerArray 都会自动同步。只要每个客户端都扩容了 Entry 数组,所有人都能看到 8 人。但 Entry 扩容是本地操作,需要每个客户端的 cheat 都做。

---

## 四、实施阶段

### 阶段 1:改人数选择条(仅 host 本地可见)

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

**说明**:`USlider` 的 `MaxValue` 是本地属性,改后滑块可拖到 8。`OpenLobby()` 会用这个 `MaxPlayers` 创建房间。仅 host 本地 UI 变化,不影响其他玩家。

### 阶段 2:底层玩家数据获取(所有客户端)

**目标**:改 `flush_player()` 从 PlayerState 取数据,不依赖 Pawn 生成。

**当前问题**:[menu.cpp:667-677](../src/cheat/menu.cpp#L667-L677) 用 `GetAllActorsOfClass(ABPCharacter_Demo_C)` 取 Pawn,Lobby 阶段玩家没 Pawn → 列表空。

**改法**:
```cpp
// 从 GameState->PlayerArray 读 PlayerState
auto* gs = world->GameState;
for (auto* ps : gs->PlayerArray) {
    FString name = ps->GetPlayerName();
    // 加入 cheat 自己的 player_list(用于覆盖菜单显示)
}
```

**说明**:保留 Pawn 列表用于"传送到我"等功能,新增 PlayerState 列表用于显示。所有客户端可读(引擎同步数据)。

### 阶段 3:Lobby + ESC 菜单 UI 刷新(host 端核心)

**目标**:host 端修改 `MaxPlayers` 和 `PlayersConnected`,让所有客户端的 Lobby/ESC 列表自动显示 N 人。

**步骤**:
1. **host 端**:改 `Lobby_GS->MaxPlayers = N`,调用 `OnRep_MaxPlayers()` 强制刷新
2. **host 端**:调用 `Lobby_GS->Update_ConnectedPlayers_Array()` 填充 `PlayersConnected`
3. **host 端**:调用 `Lobby_GS->Update_Lobby_PlayerList()` 触发 UI 更新
4. **所有客户端**:自动收到 `PlayersConnected` 同步 → `OnRep_PlayersConnected` → UI 更新

**超过 4 人的额外处理**(每个客户端各自做):
- 找到 `UW_Lobby_PlayerList_C` 实例(Lobby 菜单 + ESC 设置面板各一个)
- 用 `UWidgetBlueprintLibrary::CreateWidget` 创建额外的 `UW_Lobby_PlayerList_Entry_C`
- Add 到 `PlayerListEntries` 数组
- 调用 `RefreshPlayerList(NetIDs, MaxPlayers)` 填充数据

### 阶段 4:VOIP 左上角列表扩容(所有客户端各自)

**目标**:让 `UW_VOIP_PlayerList_C` 支持显示超过 4 人。

**步骤**:
1. 找到 `UW_VOIP_PlayerList_C` 实例
2. 用 `CreateWidget` 创建额外的 `UW_VOIP_PlayerList_Entry_C`
3. 调用 `VOIPPlayerList->AddChild(entry)` 加到 `UVerticalBox` 容器
4. hook `RefreshPlayerListings`,让它在原逻辑执行后,手动填充第 5+ 个 Entry 的数据

**说明**:`VOIPPlayerList` 是 `UVerticalBox*` 容器(不是固定数组),可 AddChild。`PlayerStates` 来自 `AGameStateBase::PlayerArray`(引擎同步),所有客户端数据一致。

### 阶段 5:接入 menu UI(启用修改人数功能)

**目标**:在 cheat 菜单加"联机房间"分类,提供人数输入框 + 创建按钮。

**步骤**:
1. menu 加"联机房间"分类
2. host 端点击"创建房间" → 调用 `session::create_session(N, 0, false)` + `session::open_lobby()`
3. host 端同步触发阶段 3 的 `MaxPlayers` 修改 + `PlayersConnected` 刷新
4. 提供手动"刷新玩家列表"按钮(调用 `Update_ConnectedPlayers_Array()`)

---

## 五、优先级与风险

### 推荐执行顺序
```
1. [阶段1] 改人数选择条(独立,低风险)
2. [阶段2] 改 flush_player 数据源(独立,低风险)
3. [阶段5] 接入 menu UI(依赖阶段1)
4. [阶段3] Lobby/ESC 列表刷新(依赖阶段5,核心功能)
5. [阶段4] VOIP 左上角扩容(独立,中等难度,可后做)
```

### 风险点
| 风险 | 应对 |
|------|------|
| host 权限判断错误 | 调用前检查 `world->AuthorityMode == AUTHORITY_Authority` |
| CreateWidget 失败 | 确保在游戏线程执行,传入正确的 Outer 和 Class |
| Entry 扩容后数据错位 | 扩容后立即调用 RefreshPlayerList,确保索引对齐 |
| OnRep 不触发 | 手动调用 `OnRep_MaxPlayers` / `OnRep_PlayersConnected` 强制刷新 |
| 人数超过游戏支持上限 | 实测游戏是否支持 8 人(可能引擎/网络层有硬限制) |

### 验证清单
- [ ] 模式选择滑块可拖到 8
- [ ] host 创建 8 人房间,其他玩家能连入
- [ ] Lobby 列表显示所有连入玩家(超过 4 个)
- [ ] ESC 菜单列表显示所有连入玩家
- [ ] 游戏内左上角列表显示所有连入玩家
- [ ] 新玩家连入时,所有客户端 UI 自动更新
- [ ] 玩家离开时,所有客户端 UI 自动更新

---

## 六、待确认问题

1. **host/client 角色**:用户是 host 开房还是加入别人房间?(影响阶段 3/5 是否可做)
2. **人数上限**:游戏引擎是否真的支持 8 人?需实测(可能 6 人/8 人有硬限制)
3. **VOIP 扩容优先级**:阶段 4 是否需要?还是先做 Lobby + ESC 菜单即可?

---

## 七、不在本次范围

- **游戏逻辑平衡**:8 人游戏的难度/出生点/任务触发不在 cheat 职责内
- **网络性能优化**:8 人同步可能增加网络负担,需游戏自己处理
- **反作弊检测**:修改 MaxPlayers 可能触发反作弊,需实测
