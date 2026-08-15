#include "menu.h"

#include "render.h"
#include "_sdk.h"
#include "gvalue.h"
#include "gui.h"
#include "entity.h"
#include "item.h"
#include "visual.h"
#include "config.h"
#include "color.h"
#include "resource.h"
#include "gdefine.h"
#include "kismet.h"
#include "session.h"

#include <format>
#include <fstream>
#include <filesystem>


menu* menu::get()
{
	static menu inst;
	return &inst;
}

void unzip_png(const LPCWSTR& name, const char* file_path)
{
	HRSRC h_res = FindResource(gvalue::dll_inst, name, L"PNG");
	HGLOBAL h_data = LoadResource(gvalue::dll_inst, h_res);
	void* p_data = LockResource(h_data);
	DWORD size = SizeofResource(gvalue::dll_inst, h_res);
	std::ofstream out(file_path, std::ios::binary);
	out.write(reinterpret_cast<char*>(p_data), size);
	out.close();
}

void menu::init()
{
	unzip_png(MAKEINTRESOURCE(IDB_PNG1), paths::data_file("visual.png").c_str());
	unzip_png(MAKEINTRESOURCE(IDB_PNG2), paths::data_file("visual_newyear.png").c_str());

	param::t_visual = SDK::UKismetRenderingLibrary::ImportFileAsTexture2D(gvalue::world, paths::data_file_w("visual.png").c_str());
	param::t_visual_newyear = SDK::UKismetRenderingLibrary::ImportFileAsTexture2D(gvalue::world, paths::data_file_w("visual_newyear.png").c_str());
}

void menu::main()
{
	online_tick();

	if (gvalue::menu_open)
	{
		color::get()->flush_color();
		pre_base();
		base();
		cursor();
	}
	lable();
	online_hud();
}

void menu::lable()
{
	if (!gvalue::draw_lable)
	{
		return;
	}

	function::set_font(12);

	const std::wstring lab_text =
		std::wstring(L"按下 Ins（Insert）或 F1 显示/隐藏菜单\n按下 Del（Delete）关闭修改器\n") +
		std::wstring(L"按下 Tab 打开快捷菜单\n\n可在 [杂项] 菜单中关闭此提示");

	render::draw_text(
		gvalue::engine->TinyFont,
		lab_text.c_str(),
		SDK::FVector2D(10.0f, 10.0f),
		SDK::FVector2D(1.0f, 1.0f),
		SDK::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f,
		SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f),
		SDK::FVector2D(0.0f, 0.0f),
		false,
		false,
		true,
		SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

void menu::pre_base()
{
	SDK::FVector2D size = param::size * function::scale();
	gui::drag(
		param::pos,
		size,
		param::drag_pos, 
		param::is_drag
	);
}

void menu::base()
{
	function::set_font((int)12 * gvalue::menu_scale);

	render::fill_box(
		SDK::FVector2D(param::pos.X - 2 * gvalue::menu_scale, param::pos.Y - 2 * gvalue::menu_scale),
		SDK::FVector2D(param::size.X + 4, param::size.Y + 4) * function::scale(),
		color::get()->outline_col
	);
	render::fill_box(
		SDK::FVector2D(param::pos.X, param::pos.Y),
		SDK::FVector2D(param::size.X, param::size.Y) * function::scale(),
		color::get()->back_col
	);

	param::size = SDK::FVector2D(600, 400);

#define ETB_SWITCH(_name_) \
case e_page::_name_: \
	_name_(); \
	break; 

	switch (param::page)
	{
		ETB_SWITCH(visual);
	case e_page::player:
		__try
		{
			player();
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			param::player_list.clear();
			printf("player error : %d\n", GetExceptionCode());
		}
		break;
		ETB_SWITCH(item);
	case e_page::entity:
		__try
		{
			entity();
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			param::entity_list.clear();
			printf("entity error : %d\n", GetExceptionCode());
		}
		break;
		ETB_SWITCH(level);
		ETB_SWITCH(misc);
		ETB_SWITCH(online);
	}

	left();

#undef ETB_SWITCH
}

void menu::cursor()
{
	render::fill_box(
		SDK::FVector2D(gvalue::mouse.x - 3, gvalue::mouse.y - 3),
		SDK::FVector2D(6, 6),
		color::get()->normal_col
	);
	render::fill_box(
		SDK::FVector2D(gvalue::mouse.x - 2, gvalue::mouse.y - 2),
		SDK::FVector2D(4, 4),
		color::get()->text_col
	);
}

void menu::left()
{
	function::pice(SDK::FVector2D(0, 0), SDK::FVector2D(100, param::size.Y));

	static float current_y = 10.0f;
	const float target_y = 10 + (int)param::page * 40;
	current_y = current_y + (target_y - current_y) * 15 * gvalue::delta_time;
	render::fill_box(function::attach(10 * gvalue::menu_scale, current_y * gvalue::menu_scale), SDK::FVector2D(80, 30) * gvalue::menu_scale, color::get()->normal_col);

#define ETB_BUTTON(_page_,_name_,_ypos_) \
if (function::button_text(#_page_, SDK::FVector2D(10, _ypos_), SDK::FVector2D(80, 30), L#_name_)) \
{ \
	param::page = e_page::_page_; \
}

	ETB_BUTTON(visual, 视觉, 10);
	ETB_BUTTON(player, 玩家, 50);
	ETB_BUTTON(item, 物品, 90);
	ETB_BUTTON(entity, 实体, 130);
	ETB_BUTTON(level, 关卡, 170);
	ETB_BUTTON(misc, 杂项, 210);
	ETB_BUTTON(online, 联机, 250);

#undef ETB_BUTTON
}


void menu::online_tick()
{
	if (!gvalue::online_extend)
		return;

	if (!gvalue::world)
		return;

	const int target = 4 + static_cast<int>(12.0f * gvalue::online_player_count);

	// 创建 Session 前，游戏从这些界面字段读取 PublicConnections。只改 Lobby_GS
	// 会让 UI 显示扩容，但在线服务仍会按原上限拒绝新玩家。
	if (gvalue::world->OwningGameInstance &&
		gvalue::world->OwningGameInstance->IsA(SDK::UBP_MyGameInstance_C::StaticClass()))
	{
		auto* game_instance = static_cast<SDK::UBP_MyGameInstance_C*>(gvalue::world->OwningGameInstance);
		game_instance->MaxPlayers = target;
	}

	// Widget 树只需周期性查找；新打开的创建界面会在下一次检查中同步。
	static float next_widget_sync = 0.0f;
	next_widget_sync -= gvalue::delta_time;
	if (next_widget_sync <= 0.0f)
	{
		next_widget_sync = 0.25f;

		SDK::TArray<SDK::UUserWidget*> widgets;
		SDK::UWidgetBlueprintLibrary::GetAllWidgetsOfClass(
			gvalue::world,
			&widgets,
			SDK::TSubclassOf<SDK::UUserWidget>(SDK::UUI_Menu_ModeSelection_C::StaticClass()),
			false
		);

		for (SDK::UUserWidget* widget : widgets)
		{
			auto* mode_selection = static_cast<SDK::UUI_Menu_ModeSelection_C*>(widget);
			if (mode_selection->Slider_MaxPlayers)
			{
				mode_selection->Slider_MaxPlayers->SetMaxValue(static_cast<float>(target));
				mode_selection->Slider_MaxPlayers->SetValue(static_cast<float>(target));
			}
			mode_selection->MaxPlayers = target;
		}

		widgets = {};
		SDK::UWidgetBlueprintLibrary::GetAllWidgetsOfClass(
			gvalue::world,
			&widgets,
			SDK::TSubclassOf<SDK::UUserWidget>(SDK::UW_CreateServer_C::StaticClass()),
			false
		);

		for (SDK::UUserWidget* widget : widgets)
		{
			auto* create_server = static_cast<SDK::UW_CreateServer_C*>(widget);
			create_server->MaximumPlayers = target;
			if (create_server->Slider_MaxPlayers)
			{
				create_server->Slider_MaxPlayers->SetMaxValue(static_cast<float>(target));
				create_server->Slider_MaxPlayers->SetValue(static_cast<float>(target));
			}
			create_server->MaxPlayer = target;
		}
	}

	// Lobby 阶段还需要同步可复制的显示数据给已加入的客户端。
	if (!gvalue::world->GameState ||
		!gvalue::world->GameState->IsA(SDK::ALobby_GS_C::StaticClass()))
		return;

	auto* lobby_gs = static_cast<SDK::ALobby_GS_C*>(gvalue::world->GameState);
	if (lobby_gs->MaxPlayers != target)
	{
		lobby_gs->MaxPlayers = target;
	}
}

void menu::online_hud()
{
	// 仅在联机扩展 + HUD 开关都开启时显示
	if (!gvalue::online_extend || !gvalue::online_hud_show)
		return;

	// 用 AGameStateBase 获取玩家数（Lobby 和游戏内都适用）
	if (!gvalue::world || !gvalue::world->GameState)
		return;

	if (!gvalue::canvas || !gvalue::engine || !gvalue::engine->TinyFont)
		return;

	SDK::AGameStateBase* gs = gvalue::world->GameState;
	const int current = gs->PlayerArray.Num();

	// MaxPlayers 只在 Lobby_GS 有，进游戏后用 target 值
	int max = 4 + static_cast<int>(12.0f * gvalue::online_player_count);
	if (gs->IsA(SDK::ALobby_GS_C::StaticClass()))
	{
		max = static_cast<SDK::ALobby_GS_C*>(gs)->MaxPlayers;
	}

	const std::wstring text = L"房间人数：" + std::to_wstring(current) + L" / " + std::to_wstring(max);

	function::set_font(12);

	// 右上角显示，避免与左上角按键提示及游戏内 UMG 界面重叠遮挡
	const float hud_x = gvalue::canvas->SizeX - 250.0f;
	const float hud_y = 10.0f;

	render::draw_text(
		gvalue::engine->TinyFont,
		text.c_str(),
		SDK::FVector2D(hud_x, hud_y),
		SDK::FVector2D(1.0f, 1.0f),
		SDK::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f,
		SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.5f),
		SDK::FVector2D(1.0f, 1.0f),
		false,
		false,
		true,
		SDK::FLinearColor(0.0f, 0.0f, 0.0f, 1.0f)
	);
}
