#include "menu.h"

void menu::online()
{
	param::size.X = 650;
	param::size.Y = 400;

	function::pice(SDK::FVector2D(110, 10), SDK::FVector2D(param::size.X - 120, param::size.Y - 20));

	function::check_box(" ", SDK::FVector2D(120, 20), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::online_extend);
	function::text(SDK::FVector2D(150, 21), L"联机大厅扩展");

	if (!gvalue::online_extend)
		return;

	const int target = 4 + static_cast<int>(12.0f * gvalue::online_player_count);

	{
		const std::wstring wstr = L"目标人数：" + std::to_wstring(target);
		function::text(SDK::FVector2D(120, 60), wstr.c_str());
		function::drag_bar("online_count", SDK::FVector2D(120, 90), SDK::FVector2D(180, 20), SDK::FVector2D(10, 20), &gvalue::online_player_count);
	}

	function::check_box(" ", SDK::FVector2D(120, 120), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::online_hud_show);
	function::text(SDK::FVector2D(150, 121), L"显示房间人数");
}
