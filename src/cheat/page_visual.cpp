#include "menu.h"

void menu::visual()
{
	function::pice(SDK::FVector2D(110, 10), SDK::FVector2D(param::size.X - 120, 210));
	function::pice(SDK::FVector2D(110, 230), SDK::FVector2D(200, param::size.Y - 240));
	function::pice(SDK::FVector2D(320, 230), SDK::FVector2D(270, param::size.Y - 240));

#define ETB_TEXT(_x_,_text_) \
function::text(SDK::FVector2D(_x_ - 5, 20), L#_text_);

	ETB_TEXT(180, 启用);
	ETB_TEXT(240, 方框);
	ETB_TEXT(300, 名称);
	ETB_TEXT(360, 距离);
	ETB_TEXT(420, 连线);
	ETB_TEXT(480, 范围);

#undef ETB_TEXT

#define ETB_TEXT(_y_,_text_) \
function::text(SDK::FVector2D(120, _y_), L#_text_);

	ETB_TEXT(60, 玩家);
	ETB_TEXT(100, 实体);
	ETB_TEXT(140, 物品);
	ETB_TEXT(180, 互动);

#undef ETB_TEXT

#define ETB_CHECK(_y_,_type_) \
	function::check_box(" ", SDK::FVector2D(180, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.enable); \
	function::check_box(" ", SDK::FVector2D(240, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.box); \
	function::check_box(" ", SDK::FVector2D(300, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.name); \
	function::check_box(" ", SDK::FVector2D(360, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.distance); \
	function::check_box(" ", SDK::FVector2D(420, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.line); \
	function::check_box(" ", SDK::FVector2D(480, _y_), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::esp_##_type_.extent);

	ETB_CHECK(60, player);
	ETB_CHECK(100, entity);
	ETB_CHECK(140, item);
	ETB_CHECK(180, interact);

#undef ETB_CHECK

	{
		const int dist = 1000 * gvalue::esp_distance;
		const std::wstring wstr = L"透视距离：" + std::to_wstring(dist) + L"米";
		function::text(SDK::FVector2D(120, 240), wstr.c_str());
		function::drag_bar("visual_distance", SDK::FVector2D(120, 270), SDK::FVector2D(180, 20), SDK::FVector2D(10, 20), &gvalue::esp_distance);
	}

	{
		function::check_box(" ", SDK::FVector2D(120, 300), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::draw_mesh);
		function::text(SDK::FVector2D(150, 300), L"绘制周围网格体");
	}

	{
		const int dist = 1000 * gvalue::draw_mesh_distance;
		const std::wstring wstr = L"网格体距离：" + std::to_wstring(dist) + L"米";
		function::text(SDK::FVector2D(120, 330), wstr.c_str());
		function::drag_bar("draw_mesh_distance", SDK::FVector2D(120, 360), SDK::FVector2D(180, 20), SDK::FVector2D(10, 20), &gvalue::draw_mesh_distance);
	}

	{
		const int fov = 180 * gvalue::fov;
		const std::wstring wstr = L"视野：" + std::to_wstring(fov);
		function::text(SDK::FVector2D(330, 240), wstr.c_str());
		function::drag_bar("fov", SDK::FVector2D(420, 240), SDK::FVector2D(160, 20), SDK::FVector2D(10, 20), &gvalue::fov);
	}

	{
		function::check_box(" ", SDK::FVector2D(330, 270), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::disable_post);
		function::text(SDK::FVector2D(360, 270), L"禁用后期处理");
	}

	{
		function::check_box(" ", SDK::FVector2D(330, 300), SDK::FVector2D(20, 20), SDK::FVector2D(10, 10), &gvalue::third_person);
		function::text(SDK::FVector2D(360, 300), L"第三人称");
	}

	{
		function::text(SDK::FVector2D(330, 330), L"X");
		function::drag_bar("cam_x", SDK::FVector2D(350, 330), SDK::FVector2D(60, 20), SDK::FVector2D(10, 20), &gvalue::cam_x);

		function::text(SDK::FVector2D(420, 330), L"Y");
		function::drag_bar("cam_y", SDK::FVector2D(440, 330), SDK::FVector2D(60, 20), SDK::FVector2D(10, 20), &gvalue::cam_y);

		if (function::button_color_text(" ", SDK::FVector2D(520, 330), SDK::FVector2D(60, 20), L"重置"))
		{
			gvalue::cam_x = 0.5f;
			gvalue::cam_y = 0.5f;
			gvalue::cam_distance = 0.2f;
		}

		const int cam_distance = 1000 * gvalue::cam_distance;
		const std::wstring wstr = L"距离：" + std::to_wstring(cam_distance);
		function::text(SDK::FVector2D(330, 360), wstr.c_str());
		function::drag_bar("cam_distance", SDK::FVector2D(430, 360), SDK::FVector2D(150, 20), SDK::FVector2D(10, 20), &gvalue::cam_distance);
	}

	render::fill_box(
		function::attach((param::size.X + 18) * gvalue::menu_scale, -2 * gvalue::menu_scale), 
		SDK::FVector2D(404, 404)* function::scale(), 
		color::get()->outline_col
	);

	render::fill_box(
		function::attach((param::size.X + 20)* gvalue::menu_scale, 0), 
		SDK::FVector2D(400, 400)* function::scale(),
		color::get()->back_col
	);

	function::pice(SDK::FVector2D(param::size.X + 30, 10), SDK::FVector2D(380, 380));

	gvalue::canvas->K2_DrawTexture(
		gvalue::new_year_color ? param::t_visual_newyear : param::t_visual,
		function::attach(640 * gvalue::menu_scale, 20 * gvalue::menu_scale),
		SDK::FVector2D(360, 360)* function::scale(),
		SDK::FVector2D(0, 0),
		SDK::FVector2D(1, 1),
		SDK::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f),
		SDK::EBlendMode::BLEND_Translucent,
		0.0f,
		SDK::FVector2D(0, 0)
	);

	{
		auto show_esp = [&](const s_esp& esp, SDK::FVector2D pos, SDK::FVector2D size, const SDK::FLinearColor& color,const std::wstring& name)
			{
				pos *= function::scale();
				size *= function::scale();

				if (!esp.enable)
				{
					return;
				}

				const SDK::FVector2D real_pos = function::attach((param::size.X + 20) * gvalue::menu_scale, 0) + pos;

				if (esp.box||esp.extent)
				{
					render::draw_box(real_pos, size, 1.0f, color);
				}
				if (esp.line)
				{
					render::draw_line(
						SDK::FVector2D(param::pos.X + param::size.X + 215, param::pos.Y + 10), 
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
				SDK::FVector2D(250, 70),
				SDK::FVector2D(100, 210),
				SDK::FLinearColor(1.0f, 0.0f, 0.0f, 1.0f),
				L"年兽夕"
			);
			show_esp(gvalue::esp_item, SDK::FVector2D(275, 290), SDK::FVector2D(70, 60), SDK::FLinearColor(0.0f, 1.0f, 1.0f, 1.0f), L"红包");
			show_esp(gvalue::esp_interact, SDK::FVector2D(40, 20), SDK::FVector2D(60, 180), SDK::FLinearColor(1.0f, 1.0f, 0.0f, 1.0f), L"鞭炮");
			show_esp(gvalue::esp_player, SDK::FVector2D(110, 40), SDK::FVector2D(130, 320), SDK::FLinearColor(0.0f, 1.0f, 0.0f, 1.0f), L"门神：神荼");
		}
		else
		{
			show_esp(gvalue::esp_entity, SDK::FVector2D(220, 80), SDK::FVector2D(100, 140), SDK::FLinearColor(1.0f, 0.0f, 0.0f, 1.0f), L"棍母");
			show_esp(gvalue::esp_item, SDK::FVector2D(235, 270), SDK::FVector2D(80, 80), SDK::FLinearColor(0.0f, 1.0f, 1.0f, 1.0f), L"咕咕嘎嘎");
			show_esp(gvalue::esp_interact, SDK::FVector2D(35, 120), SDK::FVector2D(100, 250), SDK::FLinearColor(1.0f, 1.0f, 0.0f, 1.0f), L"吉他");
			show_esp(gvalue::esp_player, SDK::FVector2D(110, 40), SDK::FVector2D(130, 320), SDK::FLinearColor(0.0f, 1.0f, 0.0f, 1.0f), L"玩家：千早爱音");
		}
	}
}
