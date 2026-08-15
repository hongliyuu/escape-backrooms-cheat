#include "menu.h"

void menu::online()
{
	function::section(SDK::FVector2D(120, layout::TOP), SDK::FVector2D(540, 280), L"联机设置");

	function::check_box(" ", SDK::FVector2D(140, 72), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::online_extend);
	function::text(SDK::FVector2D(170, 73), L"联机大厅扩展");

	if (!gvalue::online_extend)
		return;

	const int target = 4 + static_cast<int>(12.0f * gvalue::online_player_count);

	// 房间人数相关设置归组：显示开关在上，目标人数滑块在下
	function::check_box(" ", SDK::FVector2D(140, 132), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::online_hud_show);
	function::text(SDK::FVector2D(170, 133), L"显示房间人数");

	{
		const std::wstring wstr = L"目标人数：" + std::to_wstring(target);
		function::text(SDK::FVector2D(140, 192), wstr.c_str());
		function::drag_bar("online_count", SDK::FVector2D(140, 220), SDK::FVector2D(500, 20), SDK::FVector2D(10, 20), &gvalue::online_player_count);
	}
}
