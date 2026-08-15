#include "menu.h"

#include "config.h"
#include "gdefine.h"
#include "kismet.h"

void menu::misc()
{
	// 外观
	function::section(SDK::FVector2D(120, layout::TOP), SDK::FVector2D(265, 340), L"外观");

	static const std::vector<std::wstring> theme_names = {
		L"赛博紫", L"薄荷绿", L"米白", L"冰蓝", L"深空蓝",
		L"蓝黑", L"葡萄紫", L"岩浆橙", L"暗红", L"新年特别"
	};

	int theme_index = gvalue::new_year_color ? 9 : gvalue::color_index;
	function::combo_box("theme_combo", SDK::FVector2D(135, 72), SDK::FVector2D(230, 30), theme_names, &theme_index);
	if (theme_index == 9)
	{
		gvalue::new_year_color = true;
	}
	else
	{
		gvalue::new_year_color = false;
		gvalue::color_index = theme_index;
	}

	{
		const float calc_buffer = SDK::UKismetMathLibrary::Lerp(0.0f, 4.0f, gvalue::menu_scale_buffer);
		const float truncated = floor(calc_buffer * 10) / 10.0f;
		const std::wstring show_name = L"界面尺寸：" + std::format(L"{:.1f}", truncated) + L"X";
		function::text(SDK::FVector2D(135, 122), show_name.c_str());
		function::drag_bar("menu_scale_buffer", SDK::FVector2D(135, 150), SDK::FVector2D(230, 20), SDK::FVector2D(10, 20), &gvalue::menu_scale_buffer);
		if (!kismet::is_key_down(VK_LBUTTON))
		{
			gvalue::menu_scale = truncated;
		}
	}

	// 参数与按键
	function::section(SDK::FVector2D(395, layout::TOP), SDK::FVector2D(265, 340), L"参数与按键");

	if (function::button_color_text(" ", SDK::FVector2D(410, 72), SDK::FVector2D(230, 30), L"保存参数"))
	{
		config::get()->save();
	}
	if (function::button_color_text(" ", SDK::FVector2D(410, 112), SDK::FVector2D(230, 30), L"加载参数"))
	{
		config::get()->load(paths::data_file("config.cheat"));
	}
	if (function::button_color_text(" ", SDK::FVector2D(410, 152), SDK::FVector2D(230, 30), L"打开参数配置"))
	{
		system(("start " + paths::data_file("config.cheat")).c_str());
	}

	if (function::button_color_text(" ", SDK::FVector2D(410, 210), SDK::FVector2D(230, 30), L"加载按键"))
	{
		config::get()->load(paths::data_file("keybind.cheat"));
	}
	if (function::button_color_text(" ", SDK::FVector2D(410, 250), SDK::FVector2D(230, 30), L"打开按键配置"))
	{
		system(("start " + paths::data_file("keybind.cheat")).c_str());
	}

	function::check_box(" ", SDK::FVector2D(410, 310), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::draw_lable);
	function::text(SDK::FVector2D(440, 311), L"显示按键提示");

	// 关于
	function::section(SDK::FVector2D(120, 395), SDK::FVector2D(540, 80), L"关于");
	{
		const wchar_t* log = L"逃离后室修改器\n"
			VERSION_W
			GAME_VERSION;

		function::text(SDK::FVector2D(130, 417), log);
	}
}
