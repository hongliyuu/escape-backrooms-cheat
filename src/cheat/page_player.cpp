#include "menu.h"

#include "entity.h"

void menu::player()
{
	function::pice(SDK::FVector2D(110, 10), SDK::FVector2D(200, param::size.Y - 20));
	function::pice(SDK::FVector2D(320, 10), SDK::FVector2D(270, 230));
	function::pice(SDK::FVector2D(320, 245), SDK::FVector2D(270, param::size.Y - 255));

#define ETB_CHECK(_y_,_name_,_param_) \
{ \
	function::check_box(" ", SDK::FVector2D(120, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::_param_); \
	function::text(SDK::FVector2D(150, _y_), L#_name_); \
}

#define ETB_CHECK_TEAM(_y_,_name_,_param_,_team_param_) \
{ \
	function::check_box(" ", SDK::FVector2D(120, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::_param_); \
	function::text(SDK::FVector2D(150, _y_), L#_name_); \
	function::check_box(" ", SDK::FVector2D(250, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::_team_param_); \
	function::text(SDK::FVector2D(273, _y_), L"队"); \
}

	// 玩家属性区
	ETB_CHECK_TEAM(20, 无限耐力, inf_energy, inf_energy_team);
	ETB_CHECK_TEAM(55, 无限SAN值, inf_san, inf_san_team);
	ETB_CHECK_TEAM(90, 无敌模式, inf_health, inf_health_team);
	ETB_CHECK(125, 灵魂出窍, ghost_mode);
	ETB_CHECK(160, 自动平衡, auto_balance);
	ETB_CHECK_TEAM(195, 无限跳跃, inf_jump, inf_jump_team);
	ETB_CHECK(230, 飞天遁地, fly_mode);

	{
		const int speed = 100 * gvalue::fly_speed;
		const std::wstring wstr = L"飞行速度：" + std::to_wstring(speed);
		function::text(SDK::FVector2D(120, 265), wstr.c_str());
		function::drag_bar("fly_speed", SDK::FVector2D(120, 300), SDK::FVector2D(180, 20), SDK::FVector2D(10, 20), &gvalue::fly_speed);
	}

#undef ETB_CHECK
#undef ETB_CHECK_TEAM

	// 速度区（含全队总开关）
	function::check_box(" ", SDK::FVector2D(330, 20), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::speed_team);
	function::text(SDK::FVector2D(360, 20), L"全队加速");

#define ETB_DRAG(_y_,_max_,_text_,_param_) \
{ \
	const int speed = _max_ * gvalue::_param_; \
	const std::wstring wstr = L#_text_ + std::to_wstring(speed); \
	function::text(SDK::FVector2D(330, _y_), wstr.c_str()); \
	function::drag_bar(#_param_, SDK::FVector2D(460, _y_), SDK::FVector2D(120, 20), SDK::FVector2D(10, 20), &gvalue::_param_); \
}

	ETB_DRAG(50, 2750, 行走速度：, walk_speed);
	ETB_DRAG(80, 5500, 跑步速度：, run_speed);

	{
		const std::wstring wstr = L"全局加速：" + std::format(L"{:.2f}", gvalue::global_speed * 10);
		function::text(SDK::FVector2D(330, 110), wstr.c_str());
		function::drag_bar("global_speed", SDK::FVector2D(460, 110), SDK::FVector2D(120, 20), SDK::FVector2D(10, 20), &gvalue::global_speed);
	}

	ETB_DRAG(140, 4000, 跳跃速度：, jump_speed);

	{
		const std::wstring wstr = L"空中控制：" + std::format(L"{:.2f}", gvalue::air_control * 5);
		function::text(SDK::FVector2D(330, 170), wstr.c_str());
		function::drag_bar("air_control", SDK::FVector2D(460, 170), SDK::FVector2D(120, 20), SDK::FVector2D(10, 20), &gvalue::air_control);
	}

	{
		if (function::button_color_text(" ", SDK::FVector2D(330, 200), SDK::FVector2D(250, 30), L"重置速度"))
		{
			gvalue::run_speed = 0.1f;
			gvalue::walk_speed = 0.1f;
			gvalue::global_speed = 0.1f;
			gvalue::jump_speed = 0.1f;
			gvalue::air_control = 0.01f;
		}
	}

#undef ETB_DRAG

	// 整活区
	function::check_box(" ", SDK::FVector2D(330, 255), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::spin);
	function::text(SDK::FVector2D(360, 255), L"反瞄准[整活]");

	{
		const int speed = 100 * gvalue::spin_speed;
		const std::wstring wstr = L"旋转速度：" + std::to_wstring(speed);
		function::text(SDK::FVector2D(330, 285), wstr.c_str());
		function::drag_bar("spin_speed", SDK::FVector2D(460, 285), SDK::FVector2D(120, 20), SDK::FVector2D(10, 20), &gvalue::spin_speed);
	}

	function::check_box(" ", SDK::FVector2D(330, 315), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::t_pos);
	function::text(SDK::FVector2D(360, 315), L"T字姿势[整活]");

	function::check_box(" ", SDK::FVector2D(330, 345), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::x_delete);
	function::text(SDK::FVector2D(360, 345), L"X键删除[整活]");

	function::check_box(" ", SDK::FVector2D(330, 375), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::rb_move);
	function::text(SDK::FVector2D(360, 375), L"右键移动[整活]");

	auto flush_player = [&]()
		{
			param::player_list.clear();
			if (!gvalue::world || !gvalue::world->GameState)
				return;

			SDK::TArray<SDK::AActor*> actor_list;
			SDK::UGameplayStatics::GetAllActorsOfClass(gvalue::world, SDK::ABPCharacter_Demo_C::StaticClass(), &actor_list);

			SDK::AMP_GameMode_C* game_mode = nullptr;
			if (gvalue::world->AuthorityGameMode &&
				gvalue::world->AuthorityGameMode->IsA(SDK::AMP_GameMode_C::StaticClass()))
			{
				game_mode = static_cast<SDK::AMP_GameMode_C*>(gvalue::world->AuthorityGameMode);
			}

			for (SDK::APlayerState* player_state : gvalue::world->GameState->PlayerArray)
			{
				if (!player_state)
					continue;

				s_player_entry entry{};
				entry.player_state = player_state;

				if (game_mode)
				{
					for (SDK::AMP_PlayerController_C* controller : game_mode->PlayerControllers)
					{
						if (controller && controller->PlayerState == player_state)
						{
							entry.controller = controller;
							break;
						}
					}
				}

				if (!entry.controller)
				{
					for (int index = 0; index < 16; ++index)
					{
						auto* controller = SDK::UGameplayStatics::GetPlayerController(gvalue::world, index);
						if (!controller)
							break;
						if (controller->PlayerState == player_state)
						{
							entry.controller = controller;
							break;
						}
					}
				}

				// 优先使用控制器当前 Pawn（死亡时是观战者不会误判；复活后是新角色）
				if (entry.controller && entry.controller->Pawn &&
					entry.controller->Pawn->IsA(SDK::ABPCharacter_Demo_C::StaticClass()))
				{
					auto* controller_pawn = static_cast<SDK::ABPCharacter_Demo_C*>(entry.controller->Pawn);
					if (controller_pawn->PlayerState == player_state && controller_pawn->GetController() == entry.controller)
					{
						entry.pawn = controller_pawn;
					}
				}
				if (!entry.pawn)
				{
					for (SDK::AActor* actor : actor_list)
					{
						auto* pawn = static_cast<SDK::ABPCharacter_Demo_C*>(actor);
						if (pawn && pawn->PlayerState == player_state)
						{
							entry.pawn = pawn;
							break;
						}
					}
				}

				param::player_list.emplace_back(entry);
			}
		};

	// 页面打开期间每 1 秒自动刷新一次（进入首帧即触发，等价打开即刷新）
	static float list_timer = 0.0f;
	list_timer -= gvalue::delta_time;
	if (list_timer <= 0.0f)
	{
		list_timer = 1.0f;
		flush_player();
	}

	auto player_box = [&](const s_player_entry& entry, SDK::FVector2D pos)
		{
			if (!entry.player_state)
				return;

			auto* pawn = entry.pawn;
			// 本地玩家正在控制实体时，本体并非死亡：控制权被实体占用，
			// 角色 PlayerState 被清空导致列表扫不到本体
			const bool is_controlling = entry.controller == gvalue::controller &&
				entry.controller->Pawn &&
				entry.controller->Pawn->IsA(SDK::ACharacter::StaticClass()) &&
				!entry.controller->Pawn->IsA(SDK::ABPCharacter_Demo_C::StaticClass()) &&
				!entry.controller->Pawn->IsA(SDK::ABP_Explorer_C::StaticClass()) &&
				!entry.controller->Pawn->IsA(SDK::ABP_Spectator_C::StaticClass());
			const bool is_dead = !is_controlling && (!pawn || pawn->IsDead || pawn->bIsDead);

			function::pice(pos, SDK::FVector2D(460, 40));
			function::text(pos + SDK::FVector2D(10, 12), entry.player_state->GetPlayerName());

			if (is_dead)
			{
				function::text(pos + SDK::FVector2D(180, 12), L"死亡");

				if (entry.controller && gvalue::world->AuthorityGameMode &&
					function::button_color_text(" ", pos + SDK::FVector2D(300, 5), SDK::FVector2D(80, 30), L"复活"))
				{
					// 复活自己时若正控制实体，先归还实体控制，避免状态错乱
					if (entry.controller == gvalue::controller)
						entity::get()->unposs();

					// 释放当前持有的观战者/尸体，使 RestartPlayer 满足 GetPawn()==nullptr 前提
					SDK::APawn* old_pawn = entry.controller->Pawn;
					entry.controller->UnPossess();
					if (old_pawn && old_pawn->IsA(SDK::ABP_Spectator_C::StaticClass()))
						old_pawn->K2_DestroyActor();

					// 走游戏原生出生流程：生成新角色 + Possess + ClientRestart + 游戏出生回调
					if (gvalue::world->AuthorityGameMode->IsA(SDK::AMP_GameMode_C::StaticClass()))
					{
						auto* mp_game_mode = static_cast<SDK::AMP_GameMode_C*>(gvalue::world->AuthorityGameMode);
						const bool should_spawn_spectators = mp_game_mode->ShouldSpawnSpectators;
						mp_game_mode->ShouldSpawnSpectators = false;
						mp_game_mode->RestartPlayer(entry.controller);
						mp_game_mode->ShouldSpawnSpectators = should_spawn_spectators;

						// 清除被复活玩家客户端的击杀界面（游戏自带 NetClient RPC）
						if (SDK::UFunction* func = entry.controller->Class->GetFunction("MP_PlayerController_C", "OC_RemoveKillScreen"))
						{
							entry.controller->ProcessEvent(func, nullptr);
						}
					}
					else
					{
						gvalue::world->AuthorityGameMode->RestartPlayer(entry.controller);
					}

					flush_player();
				}
			}
			else if (is_controlling)
			{
				function::text(pos + SDK::FVector2D(180, 12), L"控制实体中");
			}
			else
			{
				if (function::button_color_text(" ", pos + SDK::FVector2D(210, 5), SDK::FVector2D(40, 30), L"传送") &&
					gvalue::controller && gvalue::controller->Pawn && pawn)
				{
					gvalue::controller->Pawn->K2_SetActorLocation(pawn->K2_GetActorLocation(), false, nullptr, false);
				}

				if (function::button_color_text(" ", pos + SDK::FVector2D(260, 5), SDK::FVector2D(80, 30), L"传送到我") &&
					gvalue::controller && gvalue::controller->Pawn && pawn)
				{
					pawn->K2_SetActorLocation(gvalue::controller->Pawn->K2_GetActorLocation(), false, nullptr, false);
				}

				if (function::button_color_text(" ", pos + SDK::FVector2D(350, 5), SDK::FVector2D(40, 30), L"杀死") && pawn)
				{
					pawn->KillServer(false);
					flush_player();
				}
			}

			if (function::button_color_text(" ", pos + SDK::FVector2D(400, 5), SDK::FVector2D(50, 30), L"踢出"))
			{
				if (entry.controller && entry.controller != gvalue::controller)
				{
					SDK::UAdvancedSessionsLibrary::KickPlayer(gvalue::world, entry.controller, SDK::FText());
				}
				flush_player();
			}
		};

	render::fill_box(
		SDK::FVector2D(param::pos.X + (param::size.X + 18) * gvalue::menu_scale, param::pos.Y - 2 * gvalue::menu_scale),
		SDK::FVector2D((470 + 4)* gvalue::menu_scale, (90 + param::player_list.size() * 50 + 4)* gvalue::menu_scale), 
		color::get()->outline_col
	);

	render::fill_box(
		SDK::FVector2D(param::pos.X + (param::size.X + 20) * gvalue::menu_scale, param::pos.Y),
		SDK::FVector2D(470 * gvalue::menu_scale, (90 + param::player_list.size() * 50)* gvalue::menu_scale), 
		color::get()->back_col
	);

	if (function::button_color_text(" ", SDK::FVector2D(param::size.X + 30, 10), SDK::FVector2D(200, 30), L"将所有人传送到我"))
	{
		if (gvalue::controller && gvalue::controller->Pawn)
		{
			for (const s_player_entry& entry : param::player_list)
			{
				if (entry.pawn && !entry.pawn->IsDead && !entry.pawn->bIsDead)
				{
					entry.pawn->K2_SetActorLocation(gvalue::controller->Pawn->K2_GetActorLocation(), false, nullptr, false);
				}
			}
		}
	}

	for (int i = 0; i < param::player_list.size(); i++)
	{
		player_box(param::player_list[i], SDK::FVector2D(param::size.X + 30, 90 + 50 * i));
	}
}
