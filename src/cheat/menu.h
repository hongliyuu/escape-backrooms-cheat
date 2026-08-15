#pragma once

#include "_sdk.h"
#include "gvalue.h"
#include "render.h"
#include "gui.h"
#include "color.h"

#include <vector>
#include <string>

enum class e_page
{
	visual,
	player,
	item,
	entity,
	level,
	misc,
	online,
};

// 布局常量：所有页面共用一套网格，消除尺寸跳变与坐标碎片
namespace layout
{
	// 主面板
	constexpr float W = 680.0f;   // 主面板宽
	constexpr float H = 500.0f;   // 主面板高（全部页面统一）
	constexpr float TOP = 40.0f;  // 内容区顶部偏移（标题栏之下）

	// 控件
	constexpr float ROW = 32.0f;  // 行高
	constexpr float BTN_W = 140.0f;
	constexpr float BTN_H = 28.0f;
	constexpr float CHECK = 20.0f;

	// 内容区三列（导航宽 100，内容区 110~670）
	constexpr float COL1 = 120.0f;
	constexpr float COL2 = 300.0f;
	constexpr float COL3 = 480.0f;
	constexpr float COL_W = 170.0f;

	// 侧边导航
	constexpr float NAV_X = 10.0f;
	constexpr float NAV_W = 80.0f;
	constexpr float NAV_H = 38.0f;
}

struct s_player_entry
{
	SDK::APlayerState* player_state = nullptr;
	SDK::APlayerController* controller = nullptr;
	SDK::ABPCharacter_Demo_C* pawn = nullptr;
};

class param
{
public:
	inline static SDK::FVector2D pos = SDK::FVector2D(100.0f, 100.0f);
	inline static SDK::FVector2D size = SDK::FVector2D(600.0f, 400.0f);
	inline static SDK::FVector2D drag_pos = SDK::FVector2D(0.0f, 0.0f);
	inline static bool is_drag = false;

	inline static e_page page = e_page::visual;

	inline static std::vector<s_player_entry> player_list;
	inline static std::vector<SDK::ACharacter*> entity_list;

	inline static SDK::UTexture2D* t_visual = nullptr;
	inline static SDK::UTexture2D* t_visual_newyear = nullptr;
};

class function
{
public:
	static SDK::FVector2D scale()
	{
		return SDK::FVector2D(gvalue::menu_scale, gvalue::menu_scale);
	}

	static void set_font(const int& size)
	{
		gvalue::engine->TinyFont->LegacyFontSize = size;
	}

	static SDK::FVector2D attach(const float& ref_x, const float& ref_y)
	{
		return SDK::FVector2D(param::pos.X + ref_x, param::pos.Y + ref_y);
	}

	static bool button_color(const std::string& name, SDK::FVector2D pos, SDK::FVector2D size, bool is_attach = true)
	{
		pos *= scale();
		size *= scale();
		return gui::button_color(
			name,
			is_attach ? attach(pos.X, pos.Y) : pos, 
			SDK::FVector2D(size.X, size.Y), 
			color::get()->normal_col,
			color::get()->hover_col,
			color::get()->press_col
		);
	}

	static bool button_text(const std::string& name, SDK::FVector2D pos, SDK::FVector2D size, const SDK::FString& str, bool is_attach = true)
	{
		pos *= scale();
		size *= scale();
		return gui::button_text(
			name,
			is_attach ? attach(pos.X, pos.Y) : pos,
			SDK::FVector2D(size.X, size.Y),
			str,
			gvalue::engine->TinyFont,
			color::get()->text_col,
			color::get()->text_col,
			color::get()->text_col,
			SDK::FVector2D(1.0f, 1.0f)
		);
	}

	static bool button_color_text(const std::string& name, SDK::FVector2D pos, SDK::FVector2D size, const SDK::FString& str, bool is_attach = true)
	{
		pos *= scale();
		size *= scale();
		return gui::button_color_text(
			name,
			is_attach ? attach(pos.X, pos.Y) : pos,
			SDK::FVector2D(size.X, size.Y),
			str,
			gvalue::engine->TinyFont,
			color::get()->text_col,
			color::get()->normal_col,
			color::get()->hover_col,
			color::get()->press_col,
			SDK::FVector2D(1.0f, 1.0f)
		);
	}

	static void check_box(const std::string& name, SDK::FVector2D pos, SDK::FVector2D size, SDK::FVector2D check_size, bool* ptr, bool is_attach = true)
	{
		pos *= scale();
		size *= scale();
		check_size *= scale();
		gui::check_box(
			name,
			is_attach ? attach(pos.X, pos.Y) : pos,
			SDK::FVector2D(size.X, size.Y),
			SDK::FVector2D(check_size.X, check_size.Y),
			color::get()->check_col,
			color::get()->normal_col,
			color::get()->hover_col,
			color::get()->press_col,
			ptr
		);
	}

	static void drag_bar(const std::string& name, SDK::FVector2D pos, SDK::FVector2D size, SDK::FVector2D bar_size, float* ptr, bool is_attach = true)
	{
		pos *= scale();
		size *= scale();
		bar_size *= scale();
		gui::drag_bar(
			name,
			is_attach ? attach(pos.X, pos.Y) : pos,
			SDK::FVector2D(size.X, size.Y),
			SDK::FVector2D(bar_size.X, bar_size.Y),
			color::get()->bar_col,
			color::get()->normal_col,
			color::get()->hover_col,
			color::get()->press_col,
			ptr
		);
	}

	static void text(SDK::FVector2D pos, const SDK::FString& str, const bool& mid_x = false, const bool& mid_y = false, bool is_attach = true)
	{
		pos *= scale();
		render::draw_text(
			gvalue::engine->TinyFont,
			str,
			is_attach ? attach(pos.X, pos.Y) : pos,
			SDK::FVector2D(1.0f, 1.0f), 
			color::get()->text_col,
			1.0f,
			SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f),
			SDK::FVector2D(0.0f, 0.0f),
			mid_x,
			mid_y,
			false,
			SDK::FLinearColor(0.0f, 0.0f, 0.0f, 0.0f)
		);
	}

	static void pice(SDK::FVector2D pos, SDK::FVector2D size)
	{
		pos *= scale();
		size *= scale();
		render::fill_box(
			attach(pos.X, pos.Y),
			size,
			color::get()->pice_col
		);
	}

	// 分组面板：背景 + 顶部标题条，统一页面内分组样式
	static void section(SDK::FVector2D pos, SDK::FVector2D size, const SDK::FString& title)
	{
		pice(pos, size);
		render::fill_box(
			attach(pos.X * scale().X, pos.Y * scale().Y),
			SDK::FVector2D(size.X * scale().X, 22 * scale().Y),
			color::get()->normal_col
		);
		text(SDK::FVector2D(pos.X + 6, pos.Y + 3), title);
	}

	// 侧边导航按钮：当前页高亮（背景 + 左侧指示条），其余页文字半透明
	static bool nav_button(const std::string& name, e_page page, const SDK::FString& label, float y)
	{
		const bool active = (param::page == page);
		const SDK::FVector2D pos = SDK::FVector2D(layout::NAV_X, y) * scale();
		const SDK::FVector2D size = SDK::FVector2D(layout::NAV_W, layout::NAV_H) * scale();
		const SDK::FVector2D real_pos = attach(pos.X, pos.Y);

		if (active)
		{
			render::fill_box(real_pos, size, color::get()->normal_col);
			render::fill_box(real_pos, SDK::FVector2D(3.0f * scale().X, layout::NAV_H * scale().Y), color::get()->bar_col);
		}

		const SDK::FLinearColor& col = color::get()->text_col;
		return gui::button_text(
			name,
			real_pos,
			size,
			label,
			gvalue::engine->TinyFont,
			active ? col : SDK::FLinearColor(col.R, col.G, col.B, 0.65f),
			col,
			col,
			SDK::FVector2D(1.0f, 1.0f)
		);
	}

	// 下拉框：点击展开选项列表，选择后收起；点击列表外区域自动收起
	static bool combo_box(const std::string& name, SDK::FVector2D pos, SDK::FVector2D size, const std::vector<std::wstring>& items, int* index)
	{
		static std::string open;
		static bool was_down = false;
		const bool left_down = gvalue::mouse.left;
		const bool click = left_down && !was_down;
		was_down = left_down;

		const SDK::FVector2D s_pos = pos * scale();
		const SDK::FVector2D s_size = size * scale();
		const SDK::FVector2D real_pos = attach(s_pos.X, s_pos.Y);
		const float item_h = 20.0f * scale().Y;

		const bool is_open = (open == name);
		const bool on_button =
			gvalue::mouse.x > real_pos.X && gvalue::mouse.x < real_pos.X + s_size.X &&
			gvalue::mouse.y > real_pos.Y && gvalue::mouse.y < real_pos.Y + s_size.Y;

		if (click && on_button)
		{
			open = is_open ? "" : name;
		}

		// 闭合按钮
		gui::button_color_text(
			name + "_btn",
			real_pos,
			s_size,
			SDK::FString((L"主题：" + items[*index]).c_str()),
			gvalue::engine->TinyFont,
			color::get()->text_col,
			color::get()->normal_col,
			color::get()->hover_col,
			color::get()->press_col,
			SDK::FVector2D(1.0f, 1.0f)
		);

		if (is_open)
		{
			const SDK::FVector2D list_pos = SDK::FVector2D(real_pos.X, real_pos.Y + s_size.Y);
			const SDK::FVector2D list_size = SDK::FVector2D(s_size.X, item_h * (float)items.size());

			render::fill_box(list_pos, list_size, color::get()->pice_col);

			for (int i = 0; i < (int)items.size(); i++)
			{
				const SDK::FVector2D item_pos = SDK::FVector2D(list_pos.X, list_pos.Y + i * item_h);
				const bool selected = (i == *index);
				if (gui::button_color_text(
					name + "_i" + std::to_string(i),
					item_pos,
					SDK::FVector2D(s_size.X, item_h),
					SDK::FString(items[i].c_str()),
					gvalue::engine->TinyFont,
					selected ? SDK::FLinearColor(1.0f, 1.0f, 1.0f, 1.0f) : color::get()->text_col,
					selected ? color::get()->bar_col : color::get()->normal_col,
					color::get()->hover_col,
					color::get()->press_col,
					SDK::FVector2D(1.0f, 1.0f)))
				{
					*index = i;
					open = "";
				}
			}

			// 点击列表/按钮区域之外：收起
			const bool in_area =
				gvalue::mouse.x > real_pos.X && gvalue::mouse.x < real_pos.X + s_size.X &&
				gvalue::mouse.y > real_pos.Y && gvalue::mouse.y < real_pos.Y + s_size.Y + item_h * (float)items.size();
			if (click && !in_area)
			{
				open = "";
			}
		}

		return false;
	}
};

class menu
{
public:
	static menu* get();

	void init();
	void main();
	void online_tick();
	void online_hud();
private:
	menu() = default;

	void lable();
	void pre_base();
	void base();
	void cursor();
	void left();
	void visual();
	void player();
	void item();
	void entity();
	void level();
	void misc();
	void online();
};
