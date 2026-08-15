#include "menu.h"

#include "config.h"
#include "gdefine.h"
#include "kismet.h"

void menu::misc()
{
	// 外观
	function::section(SDK::FVector2D(120, layout::TOP), SDK::FVector2D(170, 300), L"外观");

	if (function::button_color_text(" ", SDK::FVector2D(135, 72), SDK::FVector2D(140, 30), color::get()->get_name().c_str()))
	{
		color::get()->change();
	}

	function::check_box(" ", SDK::FVector2D(135, 112), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::new_year_color);
	function::text(SDK::FVector2D(165, 113), L"使用新年主题");

	{
		const float calc_buffer = SDK::UKismetMathLibrary::Lerp(0.0f, 4.0f, gvalue::menu_scale_buffer);
		const float truncated = floor(calc_buffer * 10) / 10.0f;
		const std::wstring show_name = L"界面尺寸：" + std::format(L"{:.1f}", truncated) + L"X";
		function::text(SDK::FVector2D(135, 152), show_name.c_str());
		function::drag_bar("menu_scale_buffer", SDK::FVector2D(135, 180), SDK::FVector2D(140, 20), SDK::FVector2D(10, 20), &gvalue::menu_scale_buffer);
		if (!kismet::is_key_down(VK_LBUTTON))
		{
			gvalue::menu_scale = truncated;
		}
	}

	// 参数
	function::section(SDK::FVector2D(300, layout::TOP), SDK::FVector2D(170, 200), L"参数");

	if (function::button_color_text(" ", SDK::FVector2D(315, 72), SDK::FVector2D(140, 30), L"保存参数"))
	{
		config::get()->save();
	}
	if (function::button_color_text(" ", SDK::FVector2D(315, 112), SDK::FVector2D(140, 30), L"加载参数"))
	{
		config::get()->load(paths::data_file("config.cheat"));
	}
	if (function::button_color_text(" ", SDK::FVector2D(315, 152), SDK::FVector2D(140, 30), L"打开参数配置"))
	{
		system(("start " + paths::data_file("config.cheat")).c_str());
	}

	// 按键
	function::section(SDK::FVector2D(480, layout::TOP), SDK::FVector2D(170, 200), L"按键");

	if (function::button_color_text(" ", SDK::FVector2D(495, 72), SDK::FVector2D(140, 30), L"加载按键"))
	{
		config::get()->load(paths::data_file("keybind.cheat"));
	}
	if (function::button_color_text(" ", SDK::FVector2D(495, 112), SDK::FVector2D(140, 30), L"打开按键配置"))
	{
		system(("start " + paths::data_file("keybind.cheat")).c_str());
	}

	function::check_box(" ", SDK::FVector2D(495, 152), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::draw_lable);
	function::text(SDK::FVector2D(525, 153), L"显示按键提示");

	// 关于
	function::section(SDK::FVector2D(120, 380), SDK::FVector2D(530, 80), L"关于");
	{
		const wchar_t* log = L"逃离后室修改器\n"
			VERSION_W
			GAME_VERSION;

		function::text(SDK::FVector2D(130, 402), log);
	}
}
