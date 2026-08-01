#pragma once
#include <Windows.h>
#include <string>

using fn_post_render = void(__thiscall*)(void*, void*);
using fn_wnd_proc = WNDPROC;

struct s_mouse
{
	int x = 0;
	int y = 0;
	bool left = false;
	bool right = false;
};

struct s_esp
{
	bool enable = true;
	bool box = true;
	bool name = true;
	bool distance = true;
	bool line = false;
	bool extent = false;
};

namespace SDK
{
	class UWorld;
	class APlayerController;
	class UCanvas;
	class UEngine;
}

namespace gvalue
{
	inline void** vtb;
	inline HINSTANCE dll_inst;
	inline fn_post_render def_post_render;
	inline fn_wnd_proc def_wnd_proc;

	inline SDK::UWorld* world;
	inline SDK::APlayerController* controller;
	inline SDK::UCanvas* canvas;
	inline SDK::UEngine* engine;
	inline float delta_time = 0.0f;

	inline bool menu_open = true;
	inline bool quick_menu_open = false;
	inline bool is_exit = false;
	inline bool is_clean = false;
	inline s_mouse mouse = {};
	inline float x_offset = 0.0f;
	inline float y_offset = 0.0f;

	inline int color_index = 0;
	inline std::string max_version = "";
	inline std::string log = "";
	inline bool draw_lable = true;
	inline bool new_year_color = true;
	inline float menu_scale = 1.0f;
	inline float menu_scale_buffer = 0.25f;

	inline s_esp esp_player;
	inline s_esp esp_entity;
	inline s_esp esp_item;
	inline s_esp esp_interact;
	inline float esp_distance = 0.2f;
	inline bool draw_mesh = false;
	inline float draw_mesh_distance = 0.05f;
	inline float fov = 0.67f;
	inline bool disable_post = false;
	inline bool third_person = false;
	inline float cam_x = 0.5f;
	inline float cam_y = 0.5f;
	inline float cam_distance = 0.2f;

	inline bool inf_energy = true;
	inline bool inf_san = false;
	inline bool inf_health = false;
	inline bool ghost_mode = false;
	inline bool auto_balance = false;
	inline bool inf_jump = false;
	inline bool fly_mode = false;
	inline float fly_speed = 0.1f;
	inline bool x_delete = false;
	inline float walk_speed = 0.1f;
	inline float run_speed = 0.1f;
	inline float global_speed = 0.1f;
	inline float jump_speed = 0.1f;
	inline float air_control = 0.01f;
	inline bool spin = false;
	inline float spin_speed = 0.1f;
	inline bool t_pos = false;
	inline bool rb_move = false;

	// 全队开关（仅房主有效，对所有队友应用对应效果）
	inline bool inf_energy_team = false;
	inline bool inf_san_team = false;
	inline bool inf_health_team = false;
	inline bool inf_jump_team = false;
	inline bool speed_team = false;       // 速度总开关：行走/跑步/全局/跳跃全部应用

	inline float entity_speed = 0.1f;

	// 联机大厅扩展（仅 host）
	inline bool online_extend = false;        // 开关：开启后自动扩展房间人数
	inline float online_player_count = 0.25f; // 目标人数滑块（0.0=4人 ~ 1.0=16人），默认 7 人
	inline bool online_hud_show = true;       // 开关：是否在游戏画面显示房间人数
}