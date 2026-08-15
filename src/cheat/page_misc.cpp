#include "menu.h"

#include "config.h"
#include "gdefine.h"
#include "kismet.h"

void menu::misc()
{	
	param::size.X = 650;
	param::size.Y = 550;

	function::pice(SDK::FVector2D(110, 10), SDK::FVector2D(160, 140));
	if (function::button_color_text(" ", SDK::FVector2D(120, 20), SDK::FVector2D(140, 30), color::get()->get_name().c_str()))
	{
		color::get()->change();
	}
	function::check_box(" ", SDK::FVector2D(120, 60), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::new_year_color);
	function::text(SDK::FVector2D(150, 61), L"使用新年主题");

	{
		const float calc_buffer = SDK::UKismetMathLibrary::Lerp(0.0f, 4.0f, gvalue::menu_scale_buffer);
		const float truncated = floor(calc_buffer * 10) / 10.0f;
		const std::wstring show_name = L"界面尺寸：" + std::format(L"{:.1f}", truncated) + L"X";
		function::text(SDK::FVector2D(120, 91), show_name.c_str());
		function::drag_bar("menu_scale_buffer", SDK::FVector2D(120, 120), SDK::FVector2D(140, 20), SDK::FVector2D(10, 20), &gvalue::menu_scale_buffer);
		if (!kismet::is_key_down(VK_LBUTTON))
		{
			gvalue::menu_scale = truncated;
		}
	}


	function::pice(SDK::FVector2D(110, 160), SDK::FVector2D(160, 130));
	if (function::button_color_text(" ", SDK::FVector2D(120, 170), SDK::FVector2D(140, 30), L"保存参数"))
	{
		config::get()->save();
	}
	if (function::button_color_text(" ", SDK::FVector2D(120, 210), SDK::FVector2D(140, 30), L"加载参数"))
	{
		config::get()->load(paths::data_file("config.cheat"));
	}
	if (function::button_color_text(" ", SDK::FVector2D(120, 250), SDK::FVector2D(140, 30), L"打开参数配置"))
	{
		system(("start " + paths::data_file("config.cheat")).c_str());
	}

	function::pice(SDK::FVector2D(110, 300), SDK::FVector2D(160, 90));
	if (function::button_color_text(" ", SDK::FVector2D(120, 310), SDK::FVector2D(140, 30), L"加载按键"))
	{
		config::get()->load(paths::data_file("keybind.cheat"));
	}
	if (function::button_color_text(" ", SDK::FVector2D(120, 350), SDK::FVector2D(140, 30), L"打开按键配置"))
	{
		system(("start " + paths::data_file("keybind.cheat")).c_str());
	}

	function::pice(SDK::FVector2D(110, 400), SDK::FVector2D(160, 40));
	function::check_box(" ", SDK::FVector2D(120, 410), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::draw_lable);
	function::text(SDK::FVector2D(150, 411), L"显示按键提示");

	function::pice(SDK::FVector2D(280, 10), SDK::FVector2D(param::size.X - 290, param::size.Y - 20));
	{
		const wchar_t* log = L"逃离后室修改器\n"
			VERSION_W
			GAME_VERSION;

		function::text(SDK::FVector2D(290, 20), log);
	}
}
