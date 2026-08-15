#include "menu.h"

void menu::visual()
{
	// 左上：ESP 设置表
	function::section(SDK::FVector2D(120, layout::TOP), SDK::FVector2D(380, 200), L"透视设置");

	// 表头文字按各检查框列居中（检查框 x=170/230/290/350/410/470，中心 +10）
	function::text(SDK::FVector2D(180, 64), L"启用", true);
	function::text(SDK::FVector2D(240, 64), L"方框", true);
	function::text(SDK::FVector2D(300, 64), L"名称", true);
	function::text(SDK::FVector2D(360, 64), L"距离", true);
	function::text(SDK::FVector2D(420, 64), L"连线", true);
	function::text(SDK::FVector2D(480, 64), L"范围", true);

#define ETB_TEXT(_y_,_text_) \
function::text(SDK::FVector2D(130, _y_), L#_text_);

	ETB_TEXT(88, 玩家);
	ETB_TEXT(120, 实体);
	ETB_TEXT(152, 物品);
	ETB_TEXT(184, 互动);
	ETB_TEXT(216, 门);

#undef ETB_TEXT

#define ETB_CHECK(_y_,_type_) \
	function::check_box(" ", SDK::FVector2D(170, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.enable); \
	function::check_box(" ", SDK::FVector2D(230, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.box); \
	function::check_box(" ", SDK::FVector2D(290, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.name); \
	function::check_box(" ", SDK::FVector2D(350, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.distance); \
	function::check_box(" ", SDK::FVector2D(410, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.line); \
	function::check_box(" ", SDK::FVector2D(470, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.extent);

	ETB_CHECK(88, player);
	ETB_CHECK(120, entity);
	ETB_CHECK(152, item);
	ETB_CHECK(184, interact);
	ETB_CHECK(216, door);

#undef ETB_CHECK

	// 左下：透视与网格
	function::section(SDK::FVector2D(120, 260), SDK::FVector2D(380, 200), L"透视距离");

	{
		const int dist = 1000 * gvalue::esp_distance;
		const std::wstring wstr = L"透视距离：" + std::to_wstring(dist) + L"米";
		function::text(SDK::FVector2D(130, 284), wstr.c_str());
		function::drag_bar("visual_distance", SDK::FVector2D(130, 312), SDK::FVector2D(360, 20), SDK::FVector2D(10, 20), &gvalue::esp_distance);
	}

	{
		function::check_box(" ", SDK::FVector2D(130, 356), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::draw_mesh);
		function::text(SDK::FVector2D(160, 357), L"绘制周围网格体");
	}

	{
		const int dist = 1000 * gvalue::draw_mesh_distance;
		const std::wstring wstr = L"网格体距离：" + std::to_wstring(dist) + L"米";
		function::text(SDK::FVector2D(130, 396), wstr.c_str());
		function::drag_bar("draw_mesh_distance", SDK::FVector2D(130, 424), SDK::FVector2D(360, 20), SDK::FVector2D(10, 20), &gvalue::draw_mesh_distance);
	}

	// 右上：相机设置
	function::section(SDK::FVector2D(510, layout::TOP), SDK::FVector2D(160, 420), L"相机");

	{
		const int fov = 180 * gvalue::fov;
		const std::wstring wstr = L"视野：" + std::to_wstring(fov);
		function::text(SDK::FVector2D(520, 64), wstr.c_str());
		function::drag_bar("fov", SDK::FVector2D(520, 92), SDK::FVector2D(140, 20), SDK::FVector2D(10, 20), &gvalue::fov);
	}

	{
		function::check_box(" ", SDK::FVector2D(520, 136), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::disable_post);
		function::text(SDK::FVector2D(550, 137), L"禁用后期处理");
	}

	{
		function::check_box(" ", SDK::FVector2D(520, 172), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::third_person);
		function::text(SDK::FVector2D(550, 173), L"第三人称");
	}

	{
		function::text(SDK::FVector2D(520, 212), L"X");
		function::drag_bar("cam_x", SDK::FVector2D(535, 212), SDK::FVector2D(55, 20), SDK::FVector2D(10, 20), &gvalue::cam_x);

		function::text(SDK::FVector2D(595, 212), L"Y");
		function::drag_bar("cam_y", SDK::FVector2D(610, 212), SDK::FVector2D(50, 20), SDK::FVector2D(10, 20), &gvalue::cam_y);
	}

	{
		const int cam_distance = 1000 * gvalue::cam_distance;
		const std::wstring wstr = L"距离：" + std::to_wstring(cam_distance);
		function::text(SDK::FVector2D(520, 252), wstr.c_str());
		function::drag_bar("cam_distance", SDK::FVector2D(520, 280), SDK::FVector2D(140, 20), SDK::FVector2D(10, 20), &gvalue::cam_distance);
	}

	{
		if (function::button_color_text(" ", SDK::FVector2D(520, 320), SDK::FVector2D(140, 30), L"重置"))
		{
			gvalue::cam_x = 0.5f;
			gvalue::cam_y = 0.5f;
			gvalue::cam_distance = 0.2f;
		}
	}

	// 右侧预览面板：与主面板同高无缝拼接，整体构成完整矩形
	render::fill_box(
		SDK::FVector2D(param::pos.X + (param::size.X + 18) * gvalue::menu_scale, param::pos.Y - 2 * gvalue::menu_scale),
		SDK::FVector2D((400 + 4) * gvalue::menu_scale, (param::size.Y + 4) * gvalue::menu_scale),
		color::get()->outline_col
	);

	render::fill_box(
		SDK::FVector2D(param::pos.X + (param::size.X + 20) * gvalue::menu_scale, param::pos.Y),
		SDK::FVector2D(400 * gvalue::menu_scale, param::size.Y * gvalue::menu_scale),
		color::get()->back_col
	);

	// 预览面板标题条（与主面板标题栏统一）
	render::fill_box(
		SDK::FVector2D(param::pos.X + (param::size.X + 20) * gvalue::menu_scale, param::pos.Y),
		SDK::FVector2D(400 * gvalue::menu_scale, 28 * gvalue::menu_scale),
		color::get()->normal_col
	);
	render::fill_box(
		SDK::FVector2D(param::pos.X + (param::size.X + 20) * gvalue::menu_scale, param::pos.Y + 28 * gvalue::menu_scale),
		SDK::FVector2D(400 * gvalue::menu_scale, 2 * gvalue::menu_scale),
		color::get()->outline_col
	);
	function::text(SDK::FVector2D(param::size.X + 30, 7), L"效果预览");

	function::pice(SDK::FVector2D(param::size.X + 30, 48), SDK::FVector2D(380, 380));

	gvalue::canvas->K2_DrawTexture(
		gvalue::new_year_color ? param::t_visual_newyear : param::t_visual,
		function::attach((param::size.X + 40) * gvalue::menu_scale, 58 * gvalue::menu_scale),
		SDK::FVector2D(360, 360) * function::scale(),
		SDK::FVector2D(0, 0),
		SDK::FVector2D(1, 1),
		SDK::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
		SDK::EBlendMode::BLEND_Translucent,
		0.0f,
		SDK::FVector2D(0, 0)
	);

	{
		auto show_esp = [&](const s_esp& esp, SDK::FVector2D pos, SDK::FVector2D size, const SDK::FLinearColor& color, const std::wstring& name)
			{
				pos *= function::scale();
				size *= function::scale();

				if (!esp.enable)
				{
					return;
				}

				const SDK::FVector2D real_pos = function::attach((param::size.X + 20) * gvalue::menu_scale, 0) + pos;

				if (esp.box || esp.extent)
				{
					render::draw_box(real_pos, size, 1.0f, color);
				}
				if (esp.line)
				{
					render::draw_line(
						SDK::FVector2D(param::pos.X + param::size.X + 215, param::pos.Y + 48),
						SDK::FVector2D(real_pos.X + size.X / 2, real_pos.Y),
						1.0f,
						color
					);
				}
				std::wstring draw_str;
				if (esp.name)
				{
					draw_str += name + L"\n";
				}
				if (esp.distance)
				{
					draw_str += L"10米\n";
				}
				render::draw_text(
					gvalue::engine->MediumFont,
					draw_str.c_str(),
					SDK::FVector2D(real_pos.X + size.X + 5.0f, real_pos.Y),
					SDK::FVector2D(1.0f, 1.0f),
					color,
					1.0f,
					SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f),
					SDK::FVector2D(0.0f, 0.0f),
					false,
					false,
					false,
					SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f)
				);
			};

		if (gvalue::new_year_color)
		{
			show_esp(
				gvalue::esp_entity,
				SDK::FVector2D(250, 118),
				SDK::FVector2D(100, 210),
				SDK::FLinearColor(1.0f, 0.0f, 0.0f, 1.0f),
				L"年兽夕"
			);
			show_esp(gvalue::esp_item, SDK::FVector2D(275, 338), SDK::FVector2D(70, 60), SDK::FLinearColor(0.0f, 1.0f, 1.0f, 1.0f), L"红包");
			show_esp(gvalue::esp_interact, SDK::FVector2D(40, 68), SDK::FVector2D(60, 180), SDK::FLinearColor(1.0f, 1.0f, 0.0f, 1.0f), L"鞭炮");
			show_esp(gvalue::esp_player, SDK::FVector2D(110, 88), SDK::FVector2D(130, 320), SDK::FLinearColor(0.0f, 1.0f, 0.0f, 1.0f), L"门神：神荼");
		}
		else
		{
			show_esp(gvalue::esp_entity, SDK::FVector2D(220, 128), SDK::FVector2D(100, 140), SDK::FLinearColor(1.0f, 0.0f, 0.0f, 1.0f), L"棍母");
			show_esp(gvalue::esp_item, SDK::FVector2D(235, 318), SDK::FVector2D(80, 80), SDK::FLinearColor(0.0f, 1.0f, 1.0f, 1.0f), L"咕咕嘎嘎");
			show_esp(gvalue::esp_interact, SDK::FVector2D(35, 168), SDK::FVector2D(100, 250), SDK::FLinearColor(1.0f, 1.0f, 0.0f, 1.0f), L"吉他");
			show_esp(gvalue::esp_player, SDK::FVector2D(110, 88), SDK::FVector2D(130, 320), SDK::FLinearColor(0.0f, 1.0f, 0.0f, 1.0f), L"玩家：千早爱音");
		}
	}
}
