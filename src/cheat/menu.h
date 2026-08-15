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
	inline static bool list_dirty = true;   // 页面切换/菜单打开时标记，列表页进入时刷新一次

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
