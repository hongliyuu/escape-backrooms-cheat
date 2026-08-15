#include "menu.h"

void menu::online()
{
	function::section(SDK::FVector2D(120, 60), SDK::FVector2D(540, 320), L"联机设置");

	function::check_box(" ", SDK::FVector2D(140, 90), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::online_extend);
	function::text(SDK::FVector2D(170, 91), L"联机大厅扩展");

	if (!gvalue::online_extend)
		return;

	const int target = 4 + static_cast<int>(12.0f * gvalue::online_player_count);

	{
		const std::wstring wstr = L"目标人数：" + std::to_wstring(target);
		function::text(SDK::FVector2D(140, 150), wstr.c_str());
		function::drag_bar("online_count", SDK::FVector2D(140, 178), SDK::FVector2D(300, 20), SDK::FVector2D(10, 20), &gvalue::online_player_count);
	}

	function::check_box(" ", SDK::FVector2D(140, 240), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::online_hud_show);
	function::text(SDK::FVector2D(170, 241), L"显示房间人数");
}
